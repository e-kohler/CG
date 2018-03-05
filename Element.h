#ifndef ELEMENT_H
#define ELEMENT_H

using namespace std;
#include <vector>
#include <gtk/gtk.h>

class Element {
    protected:
        vector<int> coords_;
    
    public:
        Element(vector<int> coords);
        ~Element();
        virtual void draw(cairo_t* cr) = 0;
};

class Point : public Element {
    public:
        Point(vector<int> coords);
        void draw(cairo_t* cr);
};

class Line : public Element {
    public:
        Line(vector<int> coords);
        void draw(cairo_t* cr);
};

class Polygon : public Element {
    public:
        Polygon(vector<int> coords);
        void draw(cairo_t* cr);
};
#endif