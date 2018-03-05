#ifndef ELEMENT_H
#define ELEMENT_H

using namespace std;
#include <list>
#include <gtk/gtk.h>

class Element {
    protected:
        list<int> coords_;
    
    public:
        Element();
        ~Element();
        virtual void draw(cairo_t* cr) = 0;
};

class Point : public Element {
    public:
        Point();
        void draw(cairo_t* cr);
};

class Line : public Element {
    public:
        Line();
        void draw(cairo_t* cr);
};

class Polygon : public Element {
    public:
        Polygon();
        void draw(cairo_t* cr);
};
#endif