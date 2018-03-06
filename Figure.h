#ifndef FIGURE_H
#define FIGURE_H

#include "Coord.h"
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
        virtual void draw(cairo_t* cr);
};

class Point : public Figure {
    public:
        virtual void draw(cairo_t* cr);
        Point(string name);
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