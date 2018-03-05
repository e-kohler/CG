#include "Element.h"
#include <gtk/gtk.h>
#include <iostream>

using namespace std;

Element::Element(vector<int> coords) {
    coords_ = coords;
}

Element::~Element() {}

Point::Point(vector<int> coords) // coords tem 2 elementos (1 coordenada).
        : Element(coords)
        {}

void Point::draw(cairo_t* cr) {
    cairo_move_to(cr, coords_[0]-1.0, coords_[1]-1.0);  // faz uma linha de 1 pixel no ponto. rsrs nao sabia outro de jeito de fazer
    cairo_line_to(cr, coords_[0], coords_[1]);
    cairo_stroke(cr);
}

Line::Line(vector<int> coords) // coords tem 4 elementos (2 pra cada coordenada).
        : Element(coords)
        {}

void Line::draw(cairo_t* cr) {
    cairo_move_to(cr, coords_[0], coords_[1]);
    cairo_line_to(cr, coords_[2], coords_[3]);
    cairo_stroke(cr);
}

Polygon::Polygon(vector<int> coords) // coords tem n elementos (2 pra cada coordenada).
        :Element(coords)
        {}

void Polygon::draw(cairo_t* cr) {
    for (int i = 0; i < coords_.size()-1; i+=2) {
        if (i == 0) cairo_move_to(cr, coords_[i], coords_[i+1]);
        if (i != coords_.size() -2) {
            cairo_line_to(cr, coords_[i+2], coords_[i+3]);
        } else {
            cairo_line_to(cr, coords_[0], coords_[1]);
        }
    }
    cairo_stroke(cr);
}