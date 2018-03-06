#include "Figure.h"
#include <gtk/gtk.h>
#include <iostream>

using namespace std;

Figure::Figure(string name) {
    this->name = name;
}

Figure::~Figure() {}

void Figure::draw(cairo_t* cr) {
    list<Coord>::iterator iterator = coords.begin();
    cairo_move_to(cr, iterator->getX(), iterator->getY());  // método para desenhar das linhas e polígonos
    for (; iterator != coords.end(); ++iterator) {
        float x = iterator->getX();
        float y = iterator->getY();
        cairo_line_to(cr, x, y);  // move o ponto atual para a primeira coordenada da forma, toda vez que um line_to ocorre, o ponto atual se torna o ponto ligado pelo line_to
        if (iterator == prev(coords.end())) {  // isso aqui é pra "fechar" a forma, quando chega na última coordenada, liga com a primeira
            float x = coords.front().getX();  // pega os valroes da primeira coordenada
            float y = coords.front().getY();
            cairo_line_to(cr, x, y);
        }
    }
    cairo_stroke(cr);
}

Point::Point(string name)
        : Figure(name)
        {}

void Point::draw(cairo_t* cr) {
    float x = coords.front().getX();
    float y = coords.front().getY();
    cairo_move_to(cr, x-1.0, y-1.0);
    cairo_line_to(cr, x, y);
    cairo_stroke(cr);
}

Line::Line(string name)
        : Figure(name)
        {}

Polygon::Polygon(string name)
        :Figure(name)
        {}