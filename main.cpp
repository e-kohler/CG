#include <gtk/gtk.h>
#include <iostream>
#include "Figure.h"
#include "Coord.h"

using namespace std;

GtkWidget* drawing_area;
list<Figure> figures;
View* view;

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

gboolean draw_callback (GtkWidget *widget, cairo_t *cr, gpointer data) {
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_paint(cr);

    cairo_set_line_width(cr, 1);

    // linha branca
    cairo_set_source_rgb(cr, 1, 1, 1);

    for (auto iterator = figures.begin(); iterator != figures.end(); ++iterator) {
        iterator->draw(cr, view);
    }
    cairo_stroke(cr);
    return FALSE;
}

static void activate (GtkApplication* app, gpointer user_data) {
    GtkWidget* window;
    GtkWidget* grid;
    GtkWidget* button;
    GtkWidget* viewport;

    Polygon* polig = new Polygon("tetra");  // cria as formas
    Polygon* triang = new Polygon("tri");
    Line* linha = new Line("linha");
    Point* point = new Point("ponto");

    linha->coords.push_back(Coord(6, 4));
    linha->coords.push_back(Coord(6, 1));

    polig->coords.push_back(Coord(4, 4));
    polig->coords.push_back(Coord(8, 4));
    polig->coords.push_back(Coord(9, 5));
    polig->coords.push_back(Coord(1, 8));

    triang->coords.push_back(Coord(1, 3));
    triang->coords.push_back(Coord(3, 6));
    triang->coords.push_back(Coord(4, 2));

    point->coords.push_back(Coord(100, 90));

    figures.push_back(*linha);
    figures.push_back(*polig);
    figures.push_back(*triang);

    view = new View();

    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "Janela");  // cria a janela
    gtk_window_set_default_size (GTK_WINDOW (window), 480, 480);

    grid = gtk_grid_new ();
    gtk_container_add (GTK_CONTAINER (window), grid);  // cria o grid e coloca na janela

    button = gtk_button_new_with_label ("Cima");  //cria um botão, conecta na função e coloca na grid
    g_signal_connect (button, "clicked", G_CALLBACK (move_up), NULL);
    gtk_grid_attach (GTK_GRID (grid), button, 1, 0, 1, 1);

    button = gtk_button_new_with_label ("Baixo");  //cria um botão, conecta na função e coloca na grid
    g_signal_connect (button, "clicked", G_CALLBACK (move_down), NULL);
    gtk_grid_attach (GTK_GRID (grid), button, 1, 2, 1, 1);

    button = gtk_button_new_with_label ("Dir");  //cria um botão, conecta na função e coloca na grid
    g_signal_connect (button, "clicked", G_CALLBACK (move_right), NULL);
    gtk_grid_attach (GTK_GRID (grid), button, 2, 1, 1, 1);

    button = gtk_button_new_with_label ("Esq");  //cria um botão, conecta na função e coloca na grid
    g_signal_connect (button, "clicked", G_CALLBACK (move_left), NULL);
    gtk_grid_attach (GTK_GRID (grid), button, 0, 1, 1, 1);

    button = gtk_button_new_with_label ("In");  //cria um botão, conecta na função e coloca na grid
    g_signal_connect_swapped (button, "clicked", G_CALLBACK (zoom_in), window);  // swapped é a mesma coisa que o connect, mas pode passar parâmetro, no caso o window pro destroy.
    gtk_grid_attach (GTK_GRID (grid), button, 3, 2, 1, 1);

    button = gtk_button_new_with_label ("Out");  //cria um botão, conecta na função e coloca na grid
    g_signal_connect (button, "clicked", G_CALLBACK (zoom_out), NULL);
    gtk_grid_attach (GTK_GRID (grid), button, 3, 0, 1, 1);

    drawing_area = gtk_drawing_area_new ();  // cria a área de desenho, arruma o tamanho, conecta no callback e colcoa na grid.
    gtk_widget_set_size_request (drawing_area, view->viewport.getX(), view->viewport.getY());
    g_signal_connect (G_OBJECT (drawing_area), "draw", G_CALLBACK (draw_callback), NULL);
    gtk_grid_attach (GTK_GRID (grid), drawing_area, 0, 3, 3, 4);

    gtk_widget_show_all (window);  // mostra tudo
}

int main (int argc, char **argv) {
  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}