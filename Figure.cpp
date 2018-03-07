#include "Figure.h"
#include <gtk/gtk.h>
#include <iostream>

using namespace std;

Figure::Figure(string name) {
    this->name = name;
}

Figure::~Figure() {}

void Figure::draw(cairo_t* cr, View* view) {

    auto iterator = coords.begin();

    Coord coord = view->world_to_viewport(*iterator);

    cairo_move_to(cr, coord.getX(), coord.getY());
    for (; iterator != coords.end(); ++iterator) {
        coord = view->world_to_viewport(*iterator);
        cairo_line_to(cr, coord.getX(), coord.getY());  // move o ponto atual para a primeira coordenada da forma, toda vez que um line_to ocorre, o ponto atual se torna o ponto ligado pelo line_to
        if (iterator == prev(coords.end())) {  // isso aqui é pra "fechar" a forma, quando chega na última coordenada, liga com a primeira
            coord = coords.front();  // pega os valroes da primeira coordenada
            coord = view->world_to_viewport(coord);
            cairo_line_to(cr, coord.getX(), coord.getY());
        }
    }
    cairo_stroke(cr);
}

Point::Point(string name)
        : Figure(name)
        {}

void Point::draw(cairo_t* cr, View* view) {
    float x = coords.front().getX();
    float y = coords.front().getY();
    cairo_move_to(cr, x-1.0, y-1.0);  // desenha uma reta de 1 pixel rs
    cairo_line_to(cr, x, y);
    cairo_stroke(cr);
}

Line::Line(string name)
        : Figure(name)
        {}

Polygon::Polygon(string name)
        :Figure(name)
        {}