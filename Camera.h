#ifndef CAMERA_H
#define CAMERA_H

#include "Vector2z.h"
#include "Clipped.h"
#include <list>
#include <gtk/gtk.h>

struct Camera{
    Camera();
    Vector2z pos;
    Vector2z viewport;
    Vector2z size;
    float angle;
    Vector2z clip;
    bool clip_method;
    float clip_yu;
    float clip_yd;
    float clip_xl;
    float clip_xr;
    Vector2z world_to_norm(Vector2z coord);
    Vector2z norm_to_view(Vector2z coord);
    Vector2z world_to_viewport(Vector2z coord);
    int get_rcode(Vector2z point);
    Clipped cohen_sutherland_clipper(Vector2z point1, Vector2z point2);
    Clipped liang_barsky_clipper(Vector2z point0, Vector2z point1);
    void draw_clipped(Clipped clipped, cairo_t* cr);
    void clip_and_draw_point(Vector2z point, cairo_t* cr);
    void poly_clipper(std::list<Vector2z> poly_coords, int x1, int y1, int x2, int y2);
    void sutherland_hodgeman_clipper(std::list<Vector2z> poly_coords);
    float x_intersect(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
    float y_intersect(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
};
#endif