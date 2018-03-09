#include <gtk/gtk.h>
#include <iostream>
#include "Figure.h"
#include "Coord.h"

using namespace std;

GtkWidget* drawing_area;  // canvas de desenho
list<Figure*> figures;  // lista de ponteiros de figuras pra desenhar
View* view;  // a câmera
GtkApplication *app;
GtkTextIter iter;
GtkTextBuffer* buffer;

gboolean draw_callback (GtkWidget *widget, cairo_t *cr, gpointer data);


//funções que mudam a posição (esq, dir, cima, baixo) ou tamanho da câmera(zoom in, zoom out)
static void move_up() {
    view->pos = view->pos + Coord(0, 1);
    gtk_widget_queue_draw(drawing_area);
}

static void move_down() {
    view->pos = view->pos + Coord(0, -1);
    gtk_widget_queue_draw(drawing_area);
}

static void move_left() {
    view->pos = view->pos + Coord(-1, 0);
    gtk_widget_queue_draw(drawing_area);
}

static void move_right() {
    view->pos = view->pos + Coord(1, 0);
    gtk_widget_queue_draw(drawing_area);
}

static void zoom_in() {
    view->size = view->size - Coord(1, 1);
    gtk_widget_queue_draw(drawing_area);
}

static void zoom_out() {
    view->size = view->size - Coord(-1, -1);
    gtk_widget_queue_draw(drawing_area);
}

static void add_ponto(GtkWidget** entries) {
    auto nome = gtk_entry_get_text(GTK_ENTRY(entries[3]));
    auto x = gtk_entry_get_text(GTK_ENTRY(entries[1]));
    auto y = gtk_entry_get_text(GTK_ENTRY(entries[2]));

    Point* point = new Point(nome);
    point->coords.push_back(Coord(stof(x), stof(y)));
    string nome_string(nome);
    figures.push_back(point);
    gtk_widget_queue_draw(drawing_area);
    gtk_text_buffer_get_end_iter(buffer, &iter);
    nome_string.append("\n");
    gtk_text_buffer_insert(buffer, &iter, nome_string.c_str(), -1);
    gtk_widget_destroy(GTK_WIDGET(entries[0]));
}

static void point_window() {
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
    gtk_grid_attach (GTK_GRID (grid), button, 1, 3, 1, 1);
    gtk_widget_show_all(window);
}

static void add_line(GtkWidget** entries) {
    auto nome = gtk_entry_get_text(GTK_ENTRY(entries[5]));
    auto x = gtk_entry_get_text(GTK_ENTRY(entries[1]));
    auto y = gtk_entry_get_text(GTK_ENTRY(entries[2]));
    auto x2 = gtk_entry_get_text(GTK_ENTRY(entries[3]));
    auto y2 = gtk_entry_get_text(GTK_ENTRY(entries[4]));

    string nome_string(nome);
    nome_string.append("\n");

    Line* line = new Line(nome);
    line->coords.push_back(Coord(stof(x), stof(y)));
    line->coords.push_back(Coord(stof(x2), stof(y2)));
    figures.push_back(line);
    gtk_widget_queue_draw(drawing_area);
    gtk_text_buffer_get_end_iter(buffer, &iter);
    gtk_text_buffer_insert(buffer, &iter, nome_string.c_str(), -1);
    gtk_widget_destroy(GTK_WIDGET(entries[0]));
}

static void line_window() {
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
    gtk_grid_attach(GTK_GRID(grid), label_x2, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_x2, 2, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label_y2, 1, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_y2, 2, 4, 1, 1);
    gtk_grid_attach (GTK_GRID (grid), button, 2, 5, 1, 1);
    gtk_widget_show_all(window);
}

static void polig_window() {

}

gboolean draw_callback (GtkWidget *widget, cairo_t *cr, gpointer data) {
    cairo_set_source_rgb(cr, 0, 0, 0);  //fundo preto
    cairo_paint(cr);

    cairo_set_line_width(cr, 1);
    cairo_set_source_rgb(cr, 1, 1, 1);  // linha branca

    for (auto iterator = figures.begin(); iterator != figures.end(); ++iterator) { // percorre a lista de figuras e invoca o draw de cada uma
        (*iterator)->draw(cr, view);
    }
    cairo_stroke(cr);
    return FALSE;
}

static void activate (GtkApplication* app, gpointer user_data) {
    GtkWidget* window;
    GtkWidget* grid;
    GtkWidget* button;
    GtkWidget* display_names;
    
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
    point2->coords.push_back(Coord(3, -4));

    figures.push_back(linha);  // coloca na lista global
    figures.push_back(polig);
    figures.push_back(polig2);
    figures.push_back(triang);
    figures.push_back(point);
    figures.push_back(point2);

    view = new View();

    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "Plano 2D");  // cria a janela
    gtk_window_set_default_size (GTK_WINDOW (window), 480, 480);

    grid = gtk_grid_new ();
    gtk_container_add (GTK_CONTAINER (window), grid);  // cria o grid e coloca na janela

    button = gtk_button_new_with_label ("Cima");  //cria um botão, conecta na função e coloca na grid
    g_signal_connect (button, "clicked", G_CALLBACK (move_up), NULL);
    gtk_grid_attach (GTK_GRID (grid), button, 0, 0, 1, 1);

    button = gtk_button_new_with_label ("Baixo");  //cria um botão, conecta na função e coloca na grid
    g_signal_connect (button, "clicked", G_CALLBACK (move_down), NULL);
    gtk_grid_attach (GTK_GRID (grid), button, 0, 2, 1, 1);

    button = gtk_button_new_with_label ("Dir");  //cria um botão, conecta na função e coloca na grid
    g_signal_connect (button, "clicked", G_CALLBACK (move_right), NULL);
    gtk_grid_attach (GTK_GRID (grid), button, 1, 1, 1, 1);

    button = gtk_button_new_with_label ("Esq");  //cria um botão, conecta na função e coloca na grid
    g_signal_connect (button, "clicked", G_CALLBACK (move_left), NULL);
    gtk_grid_attach (GTK_GRID (grid), button, 0, 1, 1, 1);

    button = gtk_button_new_with_label ("In");  //cria um botão, conecta na função e coloca na grid
    g_signal_connect (button, "clicked", G_CALLBACK (zoom_in), NULL);
    gtk_grid_attach (GTK_GRID (grid), button, 2, 2, 1, 1);

    button = gtk_button_new_with_label ("Out");  //cria um botão, conecta na função e coloca na grid
    g_signal_connect (button, "clicked", G_CALLBACK (zoom_out), NULL);
    gtk_grid_attach (GTK_GRID (grid), button, 2, 0, 1, 1);

    button = gtk_button_new_with_label ("Ponto");  //cria um botão, conecta na função e coloca na grid
    g_signal_connect (button, "clicked", G_CALLBACK (point_window), NULL);
    gtk_grid_attach (GTK_GRID (grid), button, 3, 4, 1, 1);

    button = gtk_button_new_with_label ("Reta");  //cria um botão, conecta na função e coloca na grid
    g_signal_connect (button, "clicked", G_CALLBACK (line_window), NULL);
    gtk_grid_attach (GTK_GRID (grid), button, 3, 3, 1, 1);

    button = gtk_button_new_with_label ("Polígono");  //cria um botão, conecta na função e coloca na grid
    g_signal_connect (button, "clicked", G_CALLBACK (polig_window), NULL);
    gtk_grid_attach (GTK_GRID (grid), button, 3, 5, 1, 1);

    drawing_area = gtk_drawing_area_new ();  // cria a área de desenho, arruma o tamanho, conecta no callback e colcoa na grid.
    gtk_widget_set_size_request (drawing_area, view->viewport.getX(), view->viewport.getY());  // o tamanho da drawing_board é o tamanho do viewport, eles são a mesma coisa
    g_signal_connect (G_OBJECT (drawing_area), "draw", G_CALLBACK (draw_callback), NULL);
    gtk_grid_attach (GTK_GRID (grid), drawing_area, 0, 3, 3, 3);

    display_names = gtk_text_view_new();
    buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (display_names));
    gtk_text_view_set_editable (GTK_TEXT_VIEW (display_names), FALSE);
    for (auto iterator = figures.begin(); iterator != figures.end(); ++iterator) {
        auto nome = (*iterator)->getName();
        nome.append("\n");
        gtk_text_buffer_get_end_iter(buffer, &iter);
        gtk_text_buffer_insert(buffer, &iter, nome.c_str(), -1);
    }
    gtk_grid_attach(GTK_GRID (grid), display_names, 4, 3, 2, 2);

    gtk_widget_show_all (window);  // mostra tudo
}

int main (int argc, char **argv) {
  int status;

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}