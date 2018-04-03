#include "GUI.h"
#include <iostream>
#include "Descriptor.h"


static void runGUI(GtkApplication* app, gpointer user_data) {
    GUI::activate(app, user_data);
    
}

int main (int argc, char **argv) {
    GtkApplication* app;
    int status;

    app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (runGUI), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    return status;
}   