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
    int get_rcode(Vector2z point);
    std::vector<Vector2z> clip_line(Vector2z point1, Vector2z point2);
    Vector2z clip_point(Vector2z point, float m);
};
#endif