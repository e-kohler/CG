#include "Figure.h"
#include <iostream>
#include <cmath> 

Figure::Figure(std::string name) {
    this->name = name;
}

Figure::~Figure() {}

std::string Figure::getName() {
    return this->name;
}

void Figure::transform(std::vector<std::vector<float> > matrix) {
    for (auto iterator = coords.begin(); iterator != coords.end(); ++iterator) {
        std::vector<float> current_coord = {iterator->getX(), iterator->getY(), 1};
        std::vector<float> result(3);
        for (int i = 0; i < 3; i++) {
            result[i] = 0;
            for (int j = 0; j < 3; j++) {
                result[i] += current_coord[j] * matrix[j][i];
            }
        }
        iterator->setX(result[0]);
        iterator->setY(result[1]);
    }
}

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

Point::Point(std::string name)
        : Figure(name)
        {}

void Point::draw(cairo_t* cr, View* view) {
    Coord coord = *coords.begin();
    coord = view->world_to_viewport(coord);
    cairo_move_to(cr, coord.getX(), coord.getY());
    cairo_arc(cr, coord.getX(), coord.getY(), 1, 0, 2*M_PI);
}

Line::Line(std::string name)
        : Figure(name)
        {}

Polygon::Polygon(std::string name)
        :Figure(name)
        {}