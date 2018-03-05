#include "Element.h"
#include <gtk/gtk.h>

Element::Element() {
}

Element::~Element() {}

Point::Point() {
}

void Point::draw(cairo_t* cr) {
    //desenha a parada
}

Line::Line() {
}

void Line::draw(cairo_t* cr) {
    cairo_set_source_rgb (cr, 255, 0, 0);
    cairo_move_to(cr, 100, 100);
    cairo_line_to(cr, 0, 0);
    cairo_stroke(cr);
}

Polygon::Polygon() {
}

void Polygon::draw(cairo_t* cr) {
    //desenha a parada
}