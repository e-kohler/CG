#include "GUI.h"
#include "Trans.h"

GtkWidget* GUI::drawing_area;  // canvas de desenho
std::list<Shape*> GUI::shapes;  // lista de ponteiros de figuras pra desenhar
Camera* GUI::camera;  // a câmera
GtkApplication* GUI::app;
GtkWidget* GUI::combo_box;
GtkBuilder* GUI::builder;

/////////////////////////////Callback de desenho/////////////////////////////

gboolean GUI::draw_callback (GtkWidget *widget, cairo_t *cr, gpointer data) {
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_paint(cr);

    cairo_set_line_width(cr, 1);
    cairo_set_source_rgb(cr, 0, 0, 0);

    for (auto iterator = shapes.begin(); iterator != shapes.end(); ++iterator) { // percorre a lista de figuras e invoca o draw de cada uma
        (*iterator)->draw(cr, camera);
    }
    cairo_stroke(cr);
    return FALSE;
}

void GUI::add_ponto(GtkWidget** entries) {
    auto nome = gtk_entry_get_text(GTK_ENTRY(entries[3]));
    auto x = gtk_entry_get_text(GTK_ENTRY(entries[1]));
    auto y = gtk_entry_get_text(GTK_ENTRY(entries[2]));

    Point* point = new Point(nome);
    point->coords.push_back(Vector2z(std::stof(x), std::stof(y)));
    std::string nome_string(nome);
    shapes.push_back(point);
    gtk_widget_queue_draw(drawing_area);
    gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo_box), nome_string.c_str());

    gtk_widget_destroy(GTK_WIDGET(entries[0]));
}

void GUI::add_line(GtkWidget** entries) {
    auto nome = gtk_entry_get_text(GTK_ENTRY(entries[5]));
    auto x = gtk_entry_get_text(GTK_ENTRY(entries[1]));
    auto y = gtk_entry_get_text(GTK_ENTRY(entries[2]));
    auto x2 = gtk_entry_get_text(GTK_ENTRY(entries[3]));
    auto y2 = gtk_entry_get_text(GTK_ENTRY(entries[4]));

    std::string nome_string(nome);

    Line* line = new Line(nome);
    line->coords.push_back(Vector2z(std::stof(x), std::stof(y)));
    line->coords.push_back(Vector2z(std::stof(x2), std::stof(y2)));
    shapes.push_back(line);
    gtk_widget_queue_draw(drawing_area);
    gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo_box), nome_string.c_str());
    gtk_widget_destroy(GTK_WIDGET(entries[0]));
}

/////////////////////////////Funções de controle de botões/////////////////////////////

void GUI::on_but_cima_clicked() {
    camera->pos = camera->pos + Vector2z(0, 1);
    gtk_widget_queue_draw(drawing_area);
}

void GUI::on_but_baix_clicked() {
    camera->pos = camera->pos + Vector2z(0, -1);
    gtk_widget_queue_draw(drawing_area);
}

void GUI::on_but_esq_clicked() {
    camera->pos = camera->pos + Vector2z(-1, 0);
    gtk_widget_queue_draw(drawing_area);
}

void GUI::on_but_dir_clicked() {
    camera->pos = camera->pos + Vector2z(1, 0);
    gtk_widget_queue_draw(drawing_area);
}

void GUI::on_but_in_clicked() {
    camera->size = camera->size - Vector2z(1, 1);
    gtk_widget_queue_draw(drawing_area);
}

void GUI::on_but_out_clicked() {
    camera->size = camera->size - Vector2z(-1, -1);
    gtk_widget_queue_draw(drawing_area);
}

void GUI::on_but_rot_cam_clicked() {
    camera->angle = camera->angle + 1;
    gtk_widget_queue_draw(drawing_area);
}

void GUI::on_but_point_clicked() {
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

void GUI::on_but_line_clicked() {
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

void GUI::on_but_polig_clicked() {
    // só na balinha
}

void GUI::on_but_tran_clicked() {
    GtkWidget* entry_x = GTK_WIDGET(gtk_builder_get_object(GTK_BUILDER(builder), "entry_x_tran"));
    GtkWidget* entry_y = GTK_WIDGET(gtk_builder_get_object(GTK_BUILDER(builder), "entry_y_tran"));

    auto x = gtk_entry_get_text(GTK_ENTRY(entry_x));
    auto y = gtk_entry_get_text(GTK_ENTRY(entry_y));

    Vector2z vector = Vector2z(std::stof(x), std::stof(y));

    auto selected_index = gtk_combo_box_get_active(GTK_COMBO_BOX(combo_box));
    auto it = shapes.begin();
    std::advance(it, selected_index); //std
    Trans::translate(*it, vector);
    gtk_widget_queue_draw(drawing_area);
}

void GUI::on_but_escal_clicked() {
    GtkWidget* entry_x = GTK_WIDGET(gtk_builder_get_object(GTK_BUILDER(builder), "entry_x_escal"));
    GtkWidget* entry_y = GTK_WIDGET(gtk_builder_get_object(GTK_BUILDER(builder), "entry_y_escal"));

    auto x = gtk_entry_get_text(GTK_ENTRY(entry_x));
    auto y = gtk_entry_get_text(GTK_ENTRY(entry_y));

    Vector2z vector = Vector2z(std::stof(x), std::stof(y));

    auto selected_index = gtk_combo_box_get_active(GTK_COMBO_BOX(combo_box));
    auto it = shapes.begin();
    std::advance(it, selected_index); //std
    Trans::scale(*it, vector);
    gtk_widget_queue_draw(drawing_area);
}

void GUI::on_but_rot_def_clicked() {
    GtkWidget* entry_angle = GTK_WIDGET(gtk_builder_get_object(GTK_BUILDER(builder), "entry_rot_def"));

    auto angle = gtk_entry_get_text(GTK_ENTRY(entry_angle));

    auto selected_index = gtk_combo_box_get_active(GTK_COMBO_BOX(combo_box));
    auto it = shapes.begin();
    std::advance(it, selected_index); //std
    Trans::rotate_default(*it, std::stof(angle));
    gtk_widget_queue_draw(drawing_area);
}

void GUI::on_but_rot_org_clicked() {
    GtkWidget* entry_angle = GTK_WIDGET(gtk_builder_get_object(GTK_BUILDER(builder), "entry_rot_org"));

    auto angle = gtk_entry_get_text(GTK_ENTRY(entry_angle));

    auto selected_index = gtk_combo_box_get_active(GTK_COMBO_BOX(combo_box));
    auto it = shapes.begin();
    std::advance(it, selected_index); //std
    Trans::rotate_by_point(*it, std::stof(angle), Vector2z(0, 0));
    gtk_widget_queue_draw(drawing_area);
}

void GUI::on_but_rot_point_clicked() {
    GtkWidget* entry_angle = GTK_WIDGET(gtk_builder_get_object(GTK_BUILDER(builder), "entry_rot_point_ang"));

    GtkWidget* entry_x = GTK_WIDGET(gtk_builder_get_object(GTK_BUILDER(builder), "entry_rot_point_x"));
    GtkWidget* entry_y = GTK_WIDGET(gtk_builder_get_object(GTK_BUILDER(builder), "entry_rot_point_y"));

    auto angle = gtk_entry_get_text(GTK_ENTRY(entry_angle));

    auto x = gtk_entry_get_text(GTK_ENTRY(entry_x));
    auto y = gtk_entry_get_text(GTK_ENTRY(entry_y));
    Vector2z point = Vector2z(std::stof(x), std::stof(y));

    auto selected_index = gtk_combo_box_get_active(GTK_COMBO_BOX(combo_box));
    auto it = shapes.begin();
    std::advance(it, selected_index); //std
    Trans::rotate_by_point(*it, std::stof(angle), point);
    gtk_widget_queue_draw(drawing_area);
}

/////////////////////////////Instacia os objetos/////////////////////////////

void GUI::activate (GtkApplication* app, gpointer user_data) {
    GtkWidget* window;
    
    Polygon* polig = new Polygon("tetra");  // cria as formas
    Polygon* polig2 = new Polygon("tetra2");
    Polygon* triang = new Polygon("tri");
    Line* linha = new Line("linha");
    Point* point = new Point("ponto");
    Point* point2 = new Point("ponto2");

    linha->coords.push_back(Vector2z(-5, 0));
    linha->coords.push_back(Vector2z(0, 5));

    polig->coords.push_back(Vector2z(1, 1));
    polig->coords.push_back(Vector2z(1, -1));
    polig->coords.push_back(Vector2z(-1, -1));
    polig->coords.push_back(Vector2z(-1, 1));

    polig2->coords.push_back(Vector2z(-2, -2));
    polig2->coords.push_back(Vector2z(-3, -4));
    polig2->coords.push_back(Vector2z(-5, -4));
    polig2->coords.push_back(Vector2z(-4, -2));

    triang->coords.push_back(Vector2z(1, 2));
    triang->coords.push_back(Vector2z(3, 1));
    triang->coords.push_back(Vector2z(4, 4));

    point->coords.push_back(Vector2z(0, 0));
    point2->coords.push_back(Vector2z(3, 3));

    shapes.push_back(linha);  // coloca na lista global
    shapes.push_back(polig);
    shapes.push_back(polig2);
    shapes.push_back(triang);
    shapes.push_back(point);
    shapes.push_back(point2);

    camera = new Camera();
    
    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "./glade/cg_top_frame.glade", NULL);  // tem que mudar isso pra rodar no moodle
    gtk_builder_add_callback_symbol(builder, "on_but_baix_clicked", on_but_baix_clicked);
    gtk_builder_add_callback_symbol(builder, "on_but_cima_clicked", on_but_cima_clicked);
    gtk_builder_add_callback_symbol(builder, "on_but_dir_clicked", on_but_dir_clicked);
    gtk_builder_add_callback_symbol(builder, "on_but_esq_clicked", on_but_esq_clicked);
    gtk_builder_add_callback_symbol(builder, "on_but_in_clicked", on_but_in_clicked);
    gtk_builder_add_callback_symbol(builder, "on_but_out_clicked", on_but_out_clicked);
    gtk_builder_add_callback_symbol(builder, "on_but_point_clicked", on_but_point_clicked);
    gtk_builder_add_callback_symbol(builder, "on_but_line_clicked", on_but_line_clicked);
    gtk_builder_add_callback_symbol(builder, "on_but_polig_clicked", on_but_polig_clicked);
    gtk_builder_add_callback_symbol(builder, "on_but_escal_clicked", on_but_escal_clicked);
    gtk_builder_add_callback_symbol(builder, "on_but_tran_clicked", on_but_tran_clicked);
    gtk_builder_add_callback_symbol(builder, "on_but_rot_def_clicked", on_but_rot_def_clicked);
    gtk_builder_add_callback_symbol(builder, "on_but_rot_org_clicked", on_but_rot_org_clicked);
    gtk_builder_add_callback_symbol(builder, "on_but_rot_point_clicked", on_but_rot_point_clicked);
    gtk_builder_add_callback_symbol(builder, "on_but_rot_cam_clicked", on_but_rot_cam_clicked);

    gtk_builder_connect_signals(builder, NULL);

    window = GTK_WIDGET(gtk_builder_get_object(GTK_BUILDER(builder), "window"));
    //gtk_window_set_default_size(GTK_WINDOW(window), 750, 480);

    drawing_area = GTK_WIDGET(gtk_builder_get_object(GTK_BUILDER(builder), "drawing_area"));  // recebe área de desenho do glade
    gtk_widget_set_size_request (drawing_area, camera->viewport.getX(), camera->viewport.getY());  // bota o tamanho de acordo com o viewport
    g_signal_connect (G_OBJECT (drawing_area), "draw", G_CALLBACK (draw_callback), NULL);

    combo_box = GTK_WIDGET(gtk_builder_get_object(GTK_BUILDER(builder), "combo_box"));
    for (auto iterator = shapes.begin(); iterator != shapes.end(); ++iterator) {
        const char *nome = (*iterator)->getName().c_str();
        gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo_box), nome);
    } 
    gtk_combo_box_set_active (GTK_COMBO_BOX (combo_box), 0);
    
    gtk_window_set_application (GTK_WINDOW (window), GTK_APPLICATION (app));

    gtk_widget_show_all (window);  // mostra tudo
}