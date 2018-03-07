#include "Figure.h"
#include <gtk/gtk.h>
#include <iostream>
#include <cmath> 

using namespace std;

Figure::Figure(string name) {
    this->name = name;
}

Figure::~Figure() {}

void Figure::draw(cairo_t* cr, View* view) {

    auto iterator = coords.begin();  // cada figura tem uma lista de coordenadas (vertices) chamada coords

    Coord coord = view->world_to_viewport(*iterator);  // coord recebe a primeira coordenada da figura transformada
    cairo_move_to(cr, coord.getX(), coord.getY());  // move pra coordenada
    for (; iterator != coords.end(); ++iterator) {  // percorre toda a lista de coordenadas, aplicando a transformação para viewport e desenhando as linhas nos resultados
        coord = view->world_to_viewport(*iterator); 
        cairo_line_to(cr, coord.getX(), coord.getY());
        if (iterator == prev(coords.end())) {  // isso aqui é pra "fechar" a forma, quando chega na última coordenada, liga com a primeira
            coord = coords.front();  // pega os valoroes da primeira coordenada
            coord = view->world_to_viewport(coord);
            cairo_line_to(cr, coord.getX(), coord.getY());
        }
    }
}

Point::Point(string name)
        : Figure(name)
        {}

void Point::draw(cairo_t* cr, View* view) {
    Coord coord = *coords.begin();
    coord = view->world_to_viewport(coord);
    cairo_move_to(cr, coord.getX(), coord.getY());
    cairo_arc(cr, coord.getX(), coord.getY(), 1, 0, 2*M_PI);
}

Line::Line(string name)
        : Figure(name)
        {}

Polygon::Polygon(string name)
        :Figure(name)
        {}