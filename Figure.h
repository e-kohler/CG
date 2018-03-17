#ifndef FIGURE_H
#define FIGURE_H

#include "Coord.h"
#include "View.h"
#include <gtk/gtk.h>
#include <list>
#include <vector>
#include <string>

class Figure {
    protected:
        std::string name;
           
    public:
        std::list<Coord> coords;
        Figure(std::string name);
        ~Figure();
        virtual void draw(cairo_t* cr, View* view);  // o draw depende do view, pois a cada redesenho, as propriedades da câmera sao levadas em conta
        std::string getName();
        virtual void transform(std::vector<std::vector<float> > matrix);
};

class Point : public Figure {
    public:
        Point(std::string name);
        void draw(cairo_t* cr, View* view) override;  // só point da override pq é diferente
};

class Line : public Figure {
    public:
        Line(std::string name);
};

class Polygon : public Figure {
    public:
        Polygon(std::string name);
};
#endif