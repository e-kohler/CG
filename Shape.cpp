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
    : Shape(name)
    {}

void Polygon::draw(cairo_t* cr, Camera* camera) {
    std::vector<Vector2z> points{};
    for (auto it = coords.begin(); it != coords.end(); it++) {
        points.push_back(*it);
    }
    camera->clip_draw_polygon(cr, points, filled);
}

BezierCurve::BezierCurve(std::string name, float step)
    : Shape(name)
    {
        this->step = step;
    }

void BezierCurve::draw(cairo_t* cr, Camera* camera) {
    generate_curve();
    std::vector<Vector2z> norm_points{};
    for (int i = 0; i < points.size(); i++) {
        norm_points.push_back(camera->world_to_norm(points[i]));
    }
    camera->clip_draw_curve(cr, norm_points);
}

void BezierCurve::generate_curve() {
    std::vector<Vector2z> coords_vector{};
    for (auto it = coords.begin(); it != coords.end(); it++) {  // em quase todas as operacoes de desenho
        coords_vector.push_back(*it);                           // tamo passando de lista pra vetor,
    }                                                           // talvez seja melhor só fazer vetor msm.
    points.clear();
    for (auto i = 0; i < coords_vector.size() - 1; i+=3) {
        float t = 0;
        auto p0 = coords_vector[i];
        auto p1 = coords_vector[i + 1];  
        auto p2 = coords_vector[i + 2];
        auto p3 = coords_vector[i + 3];
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


