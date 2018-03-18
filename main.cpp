#include "Figure.h"
#include <iostream>

GtkWidget* drawing_area;  // canvas de desenho
std::list<Figure*> figures;  // lista de ponteiros de figuras pra desenhar
View* view;  // a câmera
GtkApplication *app;
GtkWidget *combo_box;

/////////////////////////////Callback de desenho/////////////////////////////

gboolean draw_callback (GtkWidget *widget, cairo_t *cr, gpointer data) {
    cairo_set_source_rgb(cr, 1, 1, 1);  //fundo preto
    cairo_paint(cr);

    cairo_set_line_width(cr, 1);
    cairo_set_source_rgb(cr, 0, 0, 0);  // linha branca

    for (auto iterator = figures.begin(); iterator != figures.end(); ++iterator) { // percorre a lista de figuras e invoca o draw de cada uma
        (*iterator)->draw(cr, view);
    }
    cairo_stroke(cr);
    return FALSE;
}

/////////////////////////////Funções estáticas/////////////////////////////

static void add_ponto(GtkWidget** entries) {
    auto nome = gtk_entry_get_text(GTK_ENTRY(entries[3]));
    auto x = gtk_entry_get_text(GTK_ENTRY(entries[1]));
    auto y = gtk_entry_get_text(GTK_ENTRY(entries[2]));

    Point* point = new Point(nome);
    point->coords.push_back(Coord(std::stof(x), std::stof(y)));
    std::string nome_string(nome);
    figures.push_back(point);
    gtk_widget_queue_draw(drawing_area);
    gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo_box), nome_string.c_str());

    gtk_widget_destroy(GTK_WIDGET(entries[0]));
}

static void add_line(GtkWidget** entries) {
    auto nome = gtk_entry_get_text(GTK_ENTRY(entries[5]));
    auto x = gtk_entry_get_text(GTK_ENTRY(entries[1]));
    auto y = gtk_entry_get_text(GTK_ENTRY(entries[2]));
    auto x2 = gtk_entry_get_text(GTK_ENTRY(entries[3]));
    auto y2 = gtk_entry_get_text(GTK_ENTRY(entries[4]));

    std::string nome_string(nome);

    Line* line = new Line(nome);
    line->coords.push_back(Coord(std::stof(x), std::stof(y)));
    line->coords.push_back(Coord(std::stof(x2), std::stof(y2)));
    figures.push_back(line);
    gtk_widget_queue_draw(drawing_area);
    gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo_box), nome_string.c_str());
    gtk_widget_destroy(GTK_WIDGET(entries[0]));
}

static std::vector<std::vector<float> > matrix_mult(std::vector<std::vector<float> > matrix_a, std::vector<std::vector<float> > matrix_b) {
    std::vector<std::vector<float> > result(3, std::vector<float>(3, 0)); 
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result[i][j] = 0;                       // multiplica duas matrizes 3x3
            for (int k = 0; k < 3; k++) {
            result[i][j] += matrix_a[i][k] * matrix_b[k][j];
            }
        }
    }
    return result;
}

static void translate(Figure* figure, Coord vector) {
    std::vector<std::vector<float> > trans_matrix = {{1, 0, 0}, {0, 1, 0},{vector.getX(), vector.getY(), 1}};
    figure->transform(trans_matrix);  // o translate nao precisa daquelas mutiplicação de matriz, é só mover a figura msm
}

static void escalate(Figure* figure, Coord vector) {
    auto it_coords = figure->coords.begin();

    std::vector<std::vector<float> > result_matrix;

    Coord geo_middle = Coord(0,0);
    for (; it_coords != figure->coords.end(); ++it_coords) {
        geo_middle = geo_middle + *it_coords;
    }
    geo_middle = geo_middle / figure->coords.size();  // meio geométrico calculado

    std::vector<std::vector<float> > move_center_matrix;
    std::vector<std::vector<float> > move_back_matrix;
    std::vector<std::vector<float> > escalate_matrix;
    move_center_matrix = {{1, 0, 0}, {0, 1, 0}, {-geo_middle.getX(), -geo_middle.getY(), 1}};  // matriz q move a figura pro centro
    move_back_matrix = {{1, 0, 0}, {0, 1, 0}, {geo_middle.getX(), geo_middle.getY(), 1}};  // move de volta pro lugar
    escalate_matrix = {{vector.getX(), 0, 0}, {0, vector.getY(), 0}, {0, 0, 1}};  // aplica o escalonamento

    result_matrix = matrix_mult(move_center_matrix, escalate_matrix);
    result_matrix = matrix_mult(result_matrix, move_back_matrix);  // multiplica as 3

    figure->transform(result_matrix);  // aplica a transformação na figura com a matriz resultante
}

static void rotate(Figure* figure, Coord vector) {
    auto it_coords = figure->coords.begin();

    std::vector<std::vector<float> > result_matrix;

    Coord geo_middle = Coord(0,0);
    for (; it_coords != figure->coords.end(); ++it_coords) {
        geo_middle = geo_middle + *it_coords;
    }
    geo_middle = geo_middle / figure->coords.size();  // meio geométrico calculado

    std::vector<std::vector<float> > move_center_matrix;
    std::vector<std::vector<float> > move_back_matrix;
    std::vector<std::vector<float> > escalate_matrix;
    move_center_matrix = {{1, 0, 0}, {0, 1, 0}, {-geo_middle.getX(), -geo_middle.getY(), 1}};  // matriz q move a figura pro centro
    move_back_matrix = {{1, 0, 0}, {0, 1, 0}, {geo_middle.getX(), geo_middle.getY(), 1}};  // move de volta pro lugar
    escalate_matrix = {{vector.getX(), 0, 0}, {0, vector.getY(), 0}, {0, 0, 1}};  // aplica o escalonamento

    result_matrix = matrix_mult(move_center_matrix, escalate_matrix);
    result_matrix = matrix_mult(result_matrix, move_back_matrix);  // multiplica as 3

    figure->transform(result_matrix);  // aplica a transformação na figura com a matriz resultante
}

/////////////////////////////Funções de controle de botões/////////////////////////////

static void on_but_cima_clicked() {
    view->pos = view->pos + Coord(0, 1);
    gtk_widget_queue_draw(drawing_area);
}

static void on_but_baix_clicked() {
    view->pos = view->pos + Coord(0, -1);
    gtk_widget_queue_draw(drawing_area);
}

static void on_but_esq_clicked() {
    view->pos = view->pos + Coord(-1, 0);
    gtk_widget_queue_draw(drawing_area);
}

static void on_but_dir_clicked() {
    view->pos = view->pos + Coord(1, 0);
    gtk_widget_queue_draw(drawing_area);
}

static void on_but_in_clicked() {
    view->size = view->size - Coord(1, 1);
    gtk_widget_queue_draw(drawing_area);
}

static void on_but_out_clicked() {
    view->size = view->size - Coord(-1, -1);
    gtk_widget_queue_draw(drawing_area);
}

static void on_but_point_clicked() {
    GtkWidget* window;
    GtkWidget* grid;
    GtkWidget* button;

    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "Ponto");
    gtk_window_set_default_size (GTK_WINDOW(window), 100, 100);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    auto label_x = gtk_label_new("X");
    auto label_y = gtk_label_new("Y");
    auto label_nome = gtk_label_new("Nome");

    auto entry_x = gtk_entry_new();
    auto entry_y = gtk_entry_new();
    auto entry_nome = gtk_entry_new();

    static GtkWidget* entries[4];
    entries[0] = window;
    entries[1] = entry_x;
    entries[2] = entry_y;
    entries[3] = entry_nome;

    button = gtk_button_new_with_label ("Ok");
    g_signal_connect_swapped (button, "clicked", G_CALLBACK (add_ponto), entries);

    gtk_grid_attach(GTK_GRID(grid), label_nome, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_nome, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label_x, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_x, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label_y, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_y, 1, 2, 1, 1);
    gtk_grid_attach (GTK_GRID (grid), button, 0, 3, 3, 3);
    gtk_widget_show_all(window);
}

static void on_but_line_clicked() {
    GtkWidget* window;
    GtkWidget* grid;
    GtkWidget* button;

    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "Ponto");
    gtk_window_set_default_size (GTK_WINDOW(window), 100, 100);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    auto label_x = gtk_label_new("X");
    auto label_y = gtk_label_new("Y");
    auto label_x2 = gtk_label_new("X2");
    auto label_y2 = gtk_label_new("Y2");
    auto label_nome = gtk_label_new("Nome");

    auto entry_x = gtk_entry_new();
    auto entry_y = gtk_entry_new();
    auto entry_x2 = gtk_entry_new();
    auto entry_y2 = gtk_entry_new();
    auto entry_nome = gtk_entry_new();

    static GtkWidget* entries[6];
    entries[0] = window;
    entries[1] = entry_x;
    entries[2] = entry_y;
    entries[3] = entry_x2;
    entries[4] = entry_y2;
    entries[5] = entry_nome;

    button = gtk_button_new_with_label ("Ok");
    g_signal_connect_swapped (button, "clicked", G_CALLBACK (add_line), entries);

    gtk_grid_attach(GTK_GRID(grid), label_nome, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_nome, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label_x, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_x, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label_y, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_y, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label_x2, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_x2, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label_y2, 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_y2, 1, 4, 1, 1);
    gtk_grid_attach (GTK_GRID (grid), button, 0, 5, 3, 3);
    gtk_widget_show_all(window);
}

static void on_but_polig_clicked() {
    // só na balinha
}

static void on_but_translate_clicked() {
    Coord test_vector = Coord(3, -4);

    auto selected_index = gtk_combo_box_get_active(GTK_COMBO_BOX(combo_box));
    auto it = figures.begin();
    std::advance(it, selected_index); //std
    translate(*it, test_vector);
    gtk_widget_queue_draw(drawing_area);
}

static void on_but_escalate_clicked() {
    Coord test_vector = Coord(1.5, 1.5);

    auto selected_index = gtk_combo_box_get_active(GTK_COMBO_BOX(combo_box));
    auto it = figures.begin();
    std::advance(it, selected_index); //std
    escalate(*it, test_vector);
    gtk_widget_queue_draw(drawing_area);
}

/////////////////////////////Instacia os objetos/////////////////////////////

static void activate (GtkApplication* app, gpointer user_data) {
    GtkBuilder* builder;
    GtkWidget* window;
    
    Polygon* polig = new Polygon("tetra");  // cria as formas
    Polygon* polig2 = new Polygon("tetra2");
    Polygon* triang = new Polygon("tri");
    Line* linha = new Line("linha");
    Point* point = new Point("ponto");
    Point* point2 = new Point("ponto2");

    linha->coords.push_back(Coord(-5, 0));
    linha->coords.push_back(Coord(0, 5));

    polig->coords.push_back(Coord(1, 1));
    polig->coords.push_back(Coord(1, -1));
    polig->coords.push_back(Coord(-1, -1));
    polig->coords.push_back(Coord(-1, 1));

    polig2->coords.push_back(Coord(-2, -2));
    polig2->coords.push_back(Coord(-3, -4));
    polig2->coords.push_back(Coord(-5, -4));
    polig2->coords.push_back(Coord(-4, -2));

    triang->coords.push_back(Coord(1, 2));
    triang->coords.push_back(Coord(3, 1));
    triang->coords.push_back(Coord(4, 4));

    point->coords.push_back(Coord(0, 0));
    point2->coords.push_back(Coord(3, 3));

    figures.push_back(linha);  // coloca na lista global
    figures.push_back(polig);
    figures.push_back(polig2);
    figures.push_back(triang);
    figures.push_back(point);
    figures.push_back(point2);

    view = new View();
    
    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "./glade/cg_top_frame.glade", NULL);
    gtk_builder_add_callback_symbol(builder, "on_but_baix_clicked", on_but_baix_clicked);
    gtk_builder_add_callback_symbol(builder, "on_but_cima_clicked", on_but_cima_clicked);
    gtk_builder_add_callback_symbol(builder, "on_but_dir_clicked", on_but_dir_clicked);
    gtk_builder_add_callback_symbol(builder, "on_but_esq_clicked", on_but_esq_clicked);
    gtk_builder_add_callback_symbol(builder, "on_but_in_clicked", on_but_in_clicked);
    gtk_builder_add_callback_symbol(builder, "on_but_out_clicked", on_but_out_clicked);
    gtk_builder_add_callback_symbol(builder, "on_but_point_clicked", on_but_point_clicked);
    gtk_builder_add_callback_symbol(builder, "on_but_line_clicked", on_but_line_clicked);
    gtk_builder_add_callback_symbol(builder, "on_but_polig_clicked", on_but_polig_clicked);
    gtk_builder_add_callback_symbol(builder, "on_but_translate_clicked", on_but_translate_clicked);
    gtk_builder_add_callback_symbol(builder, "on_but_escalate_clicked", on_but_escalate_clicked);
    gtk_builder_connect_signals(builder, NULL);

    drawing_area = GTK_WIDGET(gtk_builder_get_object(GTK_BUILDER(builder), "drawing_area"));  // recebe área de desenho do glade
    gtk_widget_set_size_request (drawing_area, view->viewport.getX(), view->viewport.getY());  // bota o tamanho de acordo com o viewport
    g_signal_connect (G_OBJECT (drawing_area), "draw", G_CALLBACK (draw_callback), NULL);

    combo_box = GTK_WIDGET(gtk_builder_get_object(GTK_BUILDER(builder), "combo_box"));
    for (auto iterator = figures.begin(); iterator != figures.end(); ++iterator) {
        const char *nome = (*iterator)->getName().c_str();
        gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo_box), nome);
    } 
    gtk_combo_box_set_active (GTK_COMBO_BOX (combo_box), 0);

    window = GTK_WIDGET(gtk_builder_get_object(GTK_BUILDER(builder), "window"));
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);
    
    gtk_window_set_application (GTK_WINDOW (window), GTK_APPLICATION (app));

    gtk_widget_show_all (window);  // mostra tudo
}

/////////////////////////////main/////////////////////////////

int main (int argc, char **argv) {
    int status;

    gtk_init(&argc, &argv);

    app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    return status;
}