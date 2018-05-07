#include "Shape.h"
#include <iostream>
#include <cmath>

typedef std::vector<std::vector<float> > Matrix;

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

Spline::Spline(std::string name, float step)
    : Shape(name)
    {
        this->_step = step;
        this->float_matrix =
        {
        std::vector<float>{-1.0/6.0,  1.0/2.0, -1.0/2.0, 1.0/6.0},
        std::vector<float>{ 1.0/2.0, -1.0,      1.0/2.0, 0},
        std::vector<float>{-1.0/2.0,  0.0,      1.0/2.0, 0},
        std::vector<float>{ 1.0/6.0,  2.0/3.0,  1.0/6.0, 0}
	    };
    }

void Spline::draw(cairo_t* cr, Camera* camera) {
    generate_curve();
    camera->clip_draw_curve(cr, points);
}

//Matrix 4x4, vetor 4 elementos
std::vector<float> Spline::matrix_vector_mult(Matrix a, std::vector<float> b) {
	std::vector<float> c(4, 0);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            c[i] += b[j] * a[i][j];
        }
    }
	return c;
}

Matrix Spline::t_spline_matrix(float t) {
	return Matrix{
		{0, 0, 0, 1},
		{t * t * t, t * t, t, 0},
        {6 * t * t * t, 2 * t * t, 0, 0},
		{6 * t * t * t, 0, 0, 0}
	};
}

void Spline::generate_curve() {
    points.clear();

	for (int i = 0; i + 3 < world_coords.size(); i++) {

		Vector2z p0 = world_coords[i];
		Vector2z p1 = world_coords[i + 1];
		Vector2z p2 = world_coords[i + 2];
		Vector2z p3 = world_coords[i + 3];

		std::vector<float> gx{p0.getX(), p1.getX(), p2.getX(), p3.getX()};
		std::vector<float> gy{p0.getY(), p1.getY(), p2.getY(), p3.getY()};

		auto t_mat = t_spline_matrix(_step);

		auto cx = matrix_vector_mult(float_matrix, gx);
		auto cy = matrix_vector_mult(float_matrix, gy);

		auto fwdx = matrix_vector_mult(t_mat, cx);
		auto fwdy = matrix_vector_mult(t_mat, cy);

		fwd_diff(fwdx, fwdy);
	}
}

void Spline::fwd_diff(std::vector<float> fwdx, std::vector<float> fwdy) {
	int n = 1 / _step;
	float x   = fwdx[0];
	float dx  = fwdx[1];
	float d2x = fwdx[2];
	float d3x = fwdx[3];
	float y   = fwdy[0];
	float dy  = fwdy[1];
	float d2y = fwdy[2];
	float d3y = fwdy[3];
	points.push_back(Vector2z(x, y));
	for (int i = 0; i < n; i++) {
		x += dx;
		dx += d2x;
		d2x += d3x;
		y += dy;
		dy += d2y;
		d2y += d3y;
		points.push_back(Vector2z(x, y));
	}
}
