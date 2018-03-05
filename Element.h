using namespace std;
#include <list>
#include <gtk/gtk.h>

class Element {
    protected:
        list<int> coords_;
    
    public:
        Element(list<int> coords);
        ~Element();
        virtual void draw(cairo_t* cr) = 0;
};

class Point : public Element {
    public:
        Point(list<int> coords);
        void draw(cairo_t* cr);
};

class Line : public Element {
    public:
        Line(list<int> coords);
        void draw(cairo_t* cr);
};

class Polygon : public Element {
    public:
        Polygon(list<int> coords);
        void draw(cairo_t* cr);
};