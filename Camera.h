#ifndef CAMERA_H
#define CAMERA_H

#include "Vector2z.h"

struct Camera{
    Camera();
    Vector2z pos;
    Vector2z viewport;
    Vector2z size;
    float angle;
    Vector2z world_to_viewport(Vector2z coord);
};
#endif