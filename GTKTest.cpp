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
    gtk_window_set_title (GTK_WINDOW (window), "Janela");
    //gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);

    grid = gtk_grid_new ();
    gtk_container_add (GTK_CONTAINER (window), grid);

    button = gtk_button_new_with_label ("Button 1");
    g_signal_connect (button, "clicked", G_CALLBACK (on_button_click), NULL);
    gtk_grid_attach (GTK_GRID (grid), button, 0, 0, 1, 1);

    button = gtk_button_new_with_label ("Button 2");
    g_signal_connect (button, "clicked", G_CALLBACK (on_button_click), NULL);
    gtk_grid_attach (GTK_GRID (grid), button, 1, 0, 1, 1);

    button = gtk_button_new_with_label ("Quit");
    g_signal_connect_swapped (button, "clicked", G_CALLBACK (gtk_widget_destroy), window);
    gtk_grid_attach (GTK_GRID (grid), button, 0, 1, 2, 2);

    drawing_area = gtk_drawing_area_new ();
    gtk_widget_set_size_request (drawing_area, 200,200);
    g_signal_connect (G_OBJECT (drawing_area), "draw",
    G_CALLBACK (draw_callback), NULL);
    gtk_grid_attach (GTK_GRID (grid), drawing_area, 0, 3, 2, 2);

    gtk_widget_show_all (window);
}

void on_button_click() {
    cout << "Botão clicado pa \n";
}

gboolean draw_callback (GtkWidget *widget, cairo_t *cr, gpointer data) {
    guint width, height;
    GdkRGBA color;
    GtkStyleContext* context;
    list<int> lista;

    Line* linha = new Line();

    context = gtk_widget_get_style_context (widget);

    width = gtk_widget_get_allocated_width (widget);
    height = gtk_widget_get_allocated_height (widget);

    linha->draw(cr);
    cairo_set_source_rgb (cr, 0, 0, 255);
    cairo_move_to(cr, width/2, height/2);
    cairo_line_to(cr, width, height);
    cairo_stroke(cr);

    /**gtk_render_background (context, cr, 0, 0, width, height);

    cairo_arc (cr,
                width / 2.0, height / 2.0,
                MIN (width, height) / 2.0,
                0, 2 * G_PI);

    gtk_style_context_get_color (context,
                                gtk_style_context_get_state (context),
                                &color);**/

    cairo_fill (cr);

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