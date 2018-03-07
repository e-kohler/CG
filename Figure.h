#ifndef FIGURE_H
#define FIGURE_H

#include "Coord.h"
#include "View.h"
#include <gtk/gtk.h>
#include <list>
#include <string>

using namespace std;

class Figure {
    protected:
        string name;
           
    public:
        list<Coord> coords;
        Figure(string name);
        ~Figure();
        virtual void draw(cairo_t* cr, View* view);
};

class Point : public Figure {
    public:
        Point(string name);
        virtual void draw(cairo_t* cr, View* view);
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