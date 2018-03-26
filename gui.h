#ifndef GUI_H
#define GUI_H

#include "Figure.h"
#include <math.h>

namespace GUI {

    extern GtkWidget* drawing_area;  // canvas de desenho
    extern std::list<Figure*> figures;  // lista de ponteiros de figuras pra desenhar
    extern View* view;  // a câmera
    extern GtkApplication *app;
    extern GtkWidget* combo_box;
    extern GtkBuilder* builder;

    gboolean draw_callback (GtkWidget *widget, cairo_t *cr, gpointer data);

    void add_ponto(GtkWidget** entries);

    void add_line(GtkWidget** entries);

    std::vector<std::vector<float> > matrix_mult(std::vector<std::vector<float> > matrix_a, std::vector<std::vector<float> > matrix_b);

    void translate(Figure* figure, Coord vector);

    void escalate(Figure* figure, Coord vector);

    void rotate_default(Figure* figure, float angle);

    void rotate_by_point(Figure* figure, float angle, Coord vector);

    /////////////////////////////Funções de controle de botões/////////////////////////////

    void on_but_cima_clicked();

    void on_but_baix_clicked();

    void on_but_esq_clicked();

    void on_but_dir_clicked();

    void on_but_in_clicked();

    void on_but_out_clicked();

    void on_but_point_clicked();

    void on_but_line_clicked();

    void on_but_polig_clicked();

    void on_but_tran_clicked();

    void on_but_escal_clicked();

    void on_but_rot_def_clicked();

    void on_but_rot_org_clicked();

    void on_but_rot_point_clicked();

    /////////////////////////////Instacia os objetos/////////////////////////////

    void activate (GtkApplication* app, gpointer user_data);
}
#endif