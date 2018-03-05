#include "Element.h"

Element::Element(list<int> coords) {
    coords_ = coords;
}

Element::~Element() {}

Point::Point(list<int> coords) {
    Point(list<int> coords)
    : Element(coords)
    {}
}

Point::draw(cairo_t* cr) {
    //desenha a parada
}

Line::Line(list<int> coords) {
    Line(list<int> coords)
    : Element(coords)
    {}
}

Line::draw(cairo_t* cr) {
    cairo_set_source_rgb (cr, 255, 0, 0);
    cairo_move_to(cr, 100, 100);
    cairo_line_to(cr, 0, 0);
    cairo_stroke(cr);
}

Polygon::Polygon(list<int> coords) {
    Polygon(list<int> coords)
    : Element(coords)
    {}
}

Polygon::draw(cairo_t* cr) {
    //desenha a parada
}