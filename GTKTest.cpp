#include <gtk/gtk.h>
#include <iostream>
#include "Element.h"
#include <list>
using namespace std;

void on_button_click();
gboolean draw_callback (GtkWidget *widget, cairo_t *cr, gpointer data);

static void activate (GtkApplication* app, gpointer user_data) {
    GtkWidget* window;
    GtkWidget* grid;
    GtkWidget* button;
    GtkWidget* drawing_area;

    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "Janela");  // cria a janela
    //gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);

    grid = gtk_grid_new ();
    gtk_container_add (GTK_CONTAINER (window), grid);  // cria o grid e coloca na janela

    button = gtk_button_new_with_label ("Button 1");  //cria um botão, conecta na função e coloca na grid
    g_signal_connect (button, "clicked", G_CALLBACK (on_button_click), NULL);
    gtk_grid_attach (GTK_GRID (grid), button, 0, 0, 1, 1);

    button = gtk_button_new_with_label ("Button 2");  //cria um botão, conecta na função e coloca na grid
    g_signal_connect (button, "clicked", G_CALLBACK (on_button_click), NULL);
    gtk_grid_attach (GTK_GRID (grid), button, 1, 0, 1, 1);

    button = gtk_button_new_with_label ("Quit");  //cria um botão, conecta na função e coloca na grid
    g_signal_connect_swapped (button, "clicked", G_CALLBACK (gtk_widget_destroy), window);  // swapped é a mesma coisa que o connect, mas pode passar parâmetro, no caso o window pro destroy.
    gtk_grid_attach (GTK_GRID (grid), button, 0, 1, 2, 2);

    drawing_area = gtk_drawing_area_new ();  // cria a área de desenho, arruma o tamanho, conecta no callback e colcoa na grid.
    gtk_widget_set_size_request (drawing_area, 200,200);
    g_signal_connect (G_OBJECT (drawing_area), "draw",
    G_CALLBACK (draw_callback), NULL);
    gtk_grid_attach (GTK_GRID (grid), drawing_area, 0, 3, 2, 2);

    gtk_widget_show_all (window);  // mostra tudo
}

void on_button_click() {
    cout << "Botão clicado pa \n";
}

gboolean draw_callback (GtkWidget *widget, cairo_t *cr, gpointer data) {
    /*guint width, height;
    GdkRGBA color;                // não sei se vamos precisar disso
    GtkStyleContext* context;*/
    vector<int> coords_polig = {40, 40, 80, 40, 90, 50, 10, 80};  //[(40, 40), (80, 40), (90, 50), (10, 80)]
    vector<int> coords_linha = {150, 100, 170, 60};  // [(150, 100), (170, 60)]
    vector<int> coords_triang = {90, 110, 30, 110, 45, 160};  // [(90, 110), (30, 110), (45, 160)]
    vector<int> coords_point = {100, 90};

    Polygon* polig = new Polygon(coords_polig);  // cria as formas
    Polygon* triang = new Polygon(coords_triang);
    Line* linha = new Line(coords_linha);
    Point* point = new Point(coords_point);

    /*context = gtk_widget_get_style_context (widget);
    width = gtk_widget_get_allocated_width (widget);  // nem disso
    height = gtk_widget_get_allocated_height (widget);*/
    
    cairo_set_source_rgb (cr, 0, 0, 255);
    polig->draw(cr);  // chama os métodos de desenho passando o cairo_t* cr
    linha->draw(cr);
    triang->draw(cr);
    point->draw(cr);
    
    //cairo_fill (cr);
    return FALSE;
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