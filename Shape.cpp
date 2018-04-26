#include "Shape.h"
#include <iostream>
#include <cmath> 

Shape::Shape(std::string name) {
    this->name = name;
    this->filled = false;
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

    Vector2z coord1 = *iterator;  // coord recebe a primeira coordenada da figura transformada

    ++iterator;

    for (; iterator != coords.end(); ++iterator) {  // percorre toda a lista de coordenadas, aplicando a transformação para viewport e desenhando as linhas nos resultados
        Clipped clipped = Clipped(coord1, *iterator);
        camera->draw_clipped_line(clipped, cr);
        coord1 = *iterator;
        if (iterator == prev(coords.end())) {  // isso aqui é pra "fechar" a forma, quando chega na última coordenada, liga com a primeira
            auto first_coord = coords.front();
            Clipped clipped_close = Clipped(coord1, first_coord);
            camera->draw_clipped_line(clipped_close, cr);
        }
    }

    if(this->filled){
        cairo_fill(cr);
    } else {
        cairo_stroke(cr);
    }

}

Point::Point(std::string name)
        : Shape(name)
        {}

void Point::draw(cairo_t* cr, Camera* camera) {
    Vector2z point = *coords.begin();
    camera->clip_and_draw_point(point, cr);
}

Line::Line(std::string name)
        : Shape(name)
        {}

Polygon::Polygon(std::string name)
        :Shape(name)
        {}

void Polygon::draw(cairo_t* cr, Camera* camera) {
    std::vector<Vector2z> points{};
    for (auto it = coords.begin(); it != coords.end(); it++) {
        points.push_back(*it);
    }
    for (int i = 0; i < points.size(); i++) {
        std::cout << points[i] << std::endl;
    }
    camera->clip_and_draw_polygon(points, cr, filled);
}

