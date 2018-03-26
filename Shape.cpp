#include "Shape.h"
#include <iostream>
#include <cmath> 

Shape::Shape(std::string name) {
    this->name = name;
}

Shape::~Shape() {}

std::string Shape::getName() {
    return this->name;
}

void Shape::transform(std::vector<std::vector<float> > matrix) {
    for (auto iterator = coords.begin(); iterator != coords.end(); ++iterator) {
        auto result = (*iterator) * matrix;
        iterator->setX(result.getX());
        iterator->setY(result.getY());
    }
}

void Shape::draw(cairo_t* cr, Camera* camera) {

    auto iterator = coords.begin();  // cada figura tem uma lista de coordenadas (vertices) chamada coords

    Vector2z coord = camera->world_to_viewport(*iterator);  // coord recebe a primeira coordenada da figura transformada
    cairo_move_to(cr, coord.getX(), coord.getY());  // move pra coordenada
    for (; iterator != coords.end(); ++iterator) {  // percorre toda a lista de coordenadas, aplicando a transformação para viewport e desenhando as linhas nos resultados
        coord = camera->world_to_viewport(*iterator); 
        cairo_line_to(cr, coord.getX(), coord.getY());
        if (iterator == prev(coords.end())) {  // isso aqui é pra "fechar" a forma, quando chega na última coordenada, liga com a primeira
            coord = coords.front();  // pega os valoroes da primeira coordenada
            coord = camera->world_to_viewport(coord);
            cairo_line_to(cr, coord.getX(), coord.getY());
        }
    }
}

Point::Point(std::string name)
        : Shape(name)
        {}

void Point::draw(cairo_t* cr, Camera* camera) {
    Vector2z coord = *coords.begin();
    coord = camera->world_to_viewport(coord);
    cairo_move_to(cr, coord.getX(), coord.getY());
    cairo_arc(cr, coord.getX(), coord.getY(), 1, 0, 2*M_PI);
}

Line::Line(std::string name)
        : Shape(name)
        {}

Polygon::Polygon(std::string name)
        :Shape(name)
        {}