#include <gtk/gtk.h>
#include <iostream>
#include "Figure.h"
#include "Coord.h"

using namespace std;

GtkWidget* drawing_area;  // canvas de desenho
list<Figure*> figures;  // lista de ponteiros de figuras pra desenhar
View* view;  // a câmera

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
    GtkWidget* viewport;

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
    gtk_window_set_title (GTK_WINDOW (window), "Janela");  // cria a janela
    //gtk_window_set_default_size (GTK_WINDOW (window), 480, 480);

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

    drawing_area = gtk_drawing_area_new ();  // cria a área de desenho, arruma o tamanho, conecta no callback e colcoa na grid.
    gtk_widget_set_size_request (drawing_area, view->viewport.getX(), view->viewport.getY());  // o tamanho da drawing_board é o tamanho do viewport, eles são a mesma coisa
    g_signal_connect (G_OBJECT (drawing_area), "draw", G_CALLBACK (draw_callback), NULL);
    gtk_grid_attach (GTK_GRID (grid), drawing_area, 0, 3, 3, 3);

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