#ifndef TRANS_H
#define TRANS_H

#include "Figure.h"

namespace Trans {
    std::vector<std::vector<float> > translating_matrix(Vector2z vector);

    std::vector<std::vector<float> > scaling_matrix(Vector2z vector, Vector2z point);

    std::vector<std::vector<float> > rotating_matrix(float angle, Vector2z point);

    void translate(Figure* figure, Vector2z vector);

    void scale(Figure* figure, Vector2z vector);

    void rotate_default(Figure* figure, float angle);

    void rotate_by_point(Figure* figure, float angle, Vector2z vector);

}

#endif