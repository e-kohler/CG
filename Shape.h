#ifndef SHAPE_H
#define SHAPE_H

#include "Vector2z.h"
#include "Camera.h"
#include <gtk/gtk.h>
#include <list>
#include <vector>
#include <string>

class Shape {
    protected:
        std::string name;
           
    public:
        std::vector<Vector2z> coords;
        Shape(std::string name);
        ~Shape();
        virtual void draw(cairo_t* cr, Camera* camera) = 0;  // o draw depende da camera, pois a cada redesenho, as propriedades da câmera sao levadas em conta
        std::string getName();
        virtual void transform(std::vector<std::vector<float> > matrix);
};

class Point : public Shape {
    public:
        Point(std::string name);
        void draw(cairo_t* cr, Camera* camera) override;
};

class Line : public Shape {
    public:
        Line(std::string name);
        void draw(cairo_t* cr, Camera* camera) override;
};

class Polygon : public Shape {
    public:
        Polygon(std::string name);
        void draw(cairo_t* cr, Camera* camera) override;
        gboolean filled;
};

class BezierCurve : public Shape {
    public:
        BezierCurve(std::string name, float step);
        void draw(cairo_t*cr, Camera* camera) override;
        std::vector<Vector2z> points{};
        float step;
        void generate_curve();
};
#endif