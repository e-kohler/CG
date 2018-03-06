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
        Figure(string name);
        ~Figure();
        list<Coord> coords;
        virtual void draw(cairo_t* cr);
};

class Point : public Figure {
    public:
        Point(string name);
        void draw(cairo_t* cr);
};

class Line : public Figure {
    public:
        Line(string name);
        void draw(cairo_t* cr);
};

class Polygon : public Figure {
    public:
        Polygon(string name);
        void draw(cairo_t* cr);
};
#endif