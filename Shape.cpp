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

Curve::Curve(std::string name)
    : Shape(name)
    {}

void Curve::draw(cairo_t* cr, Camera* camera) {
    generate_curve();
    camera->clip_draw_curve(cr, points);
}

Bezier::Bezier(std::string name)
    : Curve(name)
    {
        this->step = 0.02;
    }

void Bezier::generate_curve() {
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

Spline::Spline(std::string name)
    : Curve(name)
    {
        this->step = 0.1;
    }

void Spline::generate_curve() {
    points.clear();
    int num_curves = world_coords.size() - 3;

    double t = step;
    double t2 = t * step;
    double t3 = t2 * step;

    double n16 = 1.0/6.0;
    double n23 = 2.0/3.0;

    for (int i = 0; i < num_curves; i++) {                                                          
        auto c1 = world_coords[i];
        auto c2 = world_coords[i+1];
        auto c3 = world_coords[i+2];
        auto c4 = world_coords[i+3];

        double ax = -n16 * c1.getX() +0.5 * c2.getX() -0.5 * c3.getX() +n16 * c4.getX();
        double bx =  0.5 * c1.getX()       -c2.getX() +0.5 * c3.getX();
        double cx = -0.5 * c1.getX()              +0.5 * c3.getX();
        double dx =  n16 * c1.getX() +n23 * c2.getX() +n16 * c3.getX();

        double delta_x  = ax * t3 + bx * t2 + cx * t;
        double delta_x3 = ax * (6 * t3);
        double delta_x2 = delta_x3 +bx * (2 * t2);

        double ay = -n16 * c1.getY() +0.5 * c2.getY() -0.5 * c3.getY() +n16 * c4.getY();
        double by =  0.5 * c1.getY()       -c2.getY() +0.5 * c3.getY();
        double cy = -0.5 * c1.getY()              +0.5 * c3.getY();
        double dy =  n16 * c1.getY() +n23 * c2.getY() +n16 * c3.getY();

        double delta_y  = ay * t3 + by * t2 + cy * t;
        double delta_y3 = ay * (6 * t3);
        double delta_y2 = delta_y3 +by * (2 * t2);

        double vx = dx, vy = dy;
        points.push_back(Vector2z(vx, vy));
        for (double t = 0.0; t < 1.0; t += step) {
            double x = vx, y = vy;

            x += delta_x;
            delta_x += delta_x2;
            delta_x2 += delta_x3;

            y += delta_y;
            delta_y += delta_y2;
            delta_y2 += delta_y3;

            points.push_back(Vector2z(x,y));
            vx = x;
            vy = y;
        }                                                    
    }
}