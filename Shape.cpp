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

Point::Point(std::string name)
        : Shape(name)
        {}

void Point::draw(cairo_t* cr, Camera* camera) {
    Vector2z point = *coords.begin();
    camera->clip_draw_point(cr, point);
}

Line::Line(std::string name)
        : Shape(name)
        {}

void Line::draw(cairo_t*cr, Camera* camera) {
    std::vector<Vector2z> points{};

    points.push_back(coords.front());
    points.push_back(coords.back());

    camera->clip_draw_line(cr, points);
    cairo_stroke(cr);
}

Polygon::Polygon(std::string name)
        :Shape(name)
        {}

void Polygon::draw(cairo_t* cr, Camera* camera) {
    std::vector<Vector2z> points{};
    for (auto it = coords.begin(); it != coords.end(); it++) {
        points.push_back(*it);
    }
    camera->clip_draw_polygon(cr, points, filled);
}

