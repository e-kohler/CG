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
    //desenha a parada
}

Polygon::Polygon(list<int> coords) {
    Polygon(list<int> coords)
    : Element(coords)
    {}
}

Polygon::draw(cairo_t* cr) {
    //desenha a parada
}