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
    for (int i = 0; i < world_coords.size(); i++) {
        auto result = world_coords[i] * matrix;
        world_coords[i].setX(result.getX());
        world_coords[i].setY(result.getY());
    }
}

Point::Point(std::string name)
    : Shape(name)
    {}

void Point::draw(cairo_t* cr, Camera* camera) {
    camera->clip_draw_point(cr, *world_coords.begin());
}

Line::Line(std::string name)
    : Shape(name)
    {}

void Line::draw(cairo_t*cr, Camera* camera) {
    camera->clip_draw_line(cr, world_coords);
    cairo_stroke(cr);
}

Polygon::Polygon(std::string name)
    : Shape(name)
    {}

void Polygon::draw(cairo_t* cr, Camera* camera) {
    camera->clip_draw_polygon(cr, world_coords, filled);
}

BezierCurve::BezierCurve(std::string name, float step)
    : Shape(name)
    {
        this->step = step;
    }

void BezierCurve::draw(cairo_t* cr, Camera* camera) {
    generate_curve();
    camera->clip_draw_curve(cr, points);
}

void BezierCurve::generate_curve() {
    points.clear();
    for (auto i = 0; i < world_coords.size() - 1; i+=3) {
        float t = 0;
        auto p0 = world_coords[i];
        auto p1 = world_coords[i + 1];
        auto p2 = world_coords[i + 2];
        auto p3 = world_coords[i + 3];
        while (t <= (1 + step / 10)) {
            float t2 = t * t;
            float t3 = t2 * t;

            float x = (-t3 +3*t2 -3*t + 1) * p0.getX()
                    + (3*t3 -6*t2 +3*t) * p1.getX()
                    + (-3*t3 +3*t2) * p2.getX()
                    + (t3) * p3.getX();

            float y = (-t3 +3*t2 -3*t + 1) * p0.getY() 
                    + (3*t3 -6*t2 +3*t) * p1.getY()
                    + (-3*t3 +3*t2) * p2.getY()
                    + (t3) * p3.getY();
            points.push_back(Vector2z(x, y));
            t += step;
        }
    }
}


