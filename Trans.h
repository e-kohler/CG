#ifndef TRANS_H
#define TRANS_H

#include "Shape.h"

namespace Trans {
    std::vector<std::vector<float> > translating_matrix(Vector2z vector);

    std::vector<std::vector<float> > scaling_matrix(Vector2z vector, Vector2z point);

    std::vector<std::vector<float> > rotating_matrix(float angle, Vector2z point);

    void translate(Shape* shape, Vector2z vector);

    void scale(Shape* shape, Vector2z vector);

    void rotate_default(Shape* shape, float angle);

    void rotate_by_point(Shape* shape, float angle, Vector2z vector);

}

#endif