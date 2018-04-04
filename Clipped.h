#ifndef CLIPPED_H
#define CLIPPED_H

#include "Vector2z.h"

struct Clipped {
    bool draw;

    Vector2z coord1;
    Vector2z coord2;

    Clipped();
    Clipped(Vector2z coord1, Vector2z coord2);
};

#endif