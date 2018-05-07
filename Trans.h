#ifndef TRANS_H
#define TRANS_H

#include "Shape.h"
#include <math.h>

typedef std::vector<std::vector<float> > Matrix;

namespace Trans {
    Matrix translating_matrix(Vector2z vector);

    Matrix scaling_matrix(Vector2z vector, Vector2z point);

    Matrix rotating_matrix(float angle, Vector2z point);

    void translate(Shape* shape, Vector2z vector);

    void scale(Shape* shape, Vector2z vector);

    void rotate_default(Shape* shape, float angle);

    void rotate_by_point(Shape* shape, float angle, Vector2z vector);
}

#endif
