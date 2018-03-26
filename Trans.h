#ifndef TRANS_H
#define TRANS_H

#include "Figure.h"

namespace Trans {
    std::vector<std::vector<float> > translating_matrix(Coord vector);

    std::vector<std::vector<float> > scaling_matrix(Coord vector, Coord point);

    std::vector<std::vector<float> > rotating_matrix(float angle, Coord point);

    void translate(Figure* figure, Coord vector);

    void scale(Figure* figure, Coord vector);

    void rotate_default(Figure* figure, float angle);

    void rotate_by_point(Figure* figure, float angle, Coord vector);

}

#endif