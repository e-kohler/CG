#ifndef CAMERA_H
#define CAMERA_H

#include "Vector2z.h"
#include "Clipped.h"
#include <gtk/gtk.h>

struct Camera{
    Camera();
    Vector2z pos;
    Vector2z viewport;
    Vector2z size;
    Vector2z clip;
    float angle;
    Vector2z world_to_norm(Vector2z coord);
    Vector2z norm_to_view(Vector2z coord);
    Vector2z world_to_viewport(Vector2z coord);
    int get_rcode(Vector2z point);
    Clipped clip_line(Vector2z point1, Vector2z point2);
    void draw_clipped(Clipped clipped, cairo_t* cr);
    void clip_and_draw_point(Vector2z point, cairo_t* cr);
};
#endif