#ifndef FIGURE_H
#define FIGURE_H

#include "Coord.h"
#include "View.h"
#include <gtk/gtk.h>
#include <list>
#include <vector>
#include <string>

using namespace std;

class Figure {
    protected:
        string name;
           
    public:
        list<Coord> coords;
        Figure(string name);
        ~Figure();
        virtual void draw(cairo_t* cr, View* view);  // o draw depende do view, pois a cada redesenho, as propriedades da câmera sao levadas em conta
        string getName();
        virtual void transform(vector<vector<float> > matrix);
};

class Point : public Figure {
    public:
        Point(string name);
        void draw(cairo_t* cr, View* view) override;  // só point da override pq é diferente
};

class Line : public Figure {
    public:
        Line(string name);
};

class Polygon : public Figure {
    public:
        Polygon(string name);
};
#endif