#ifndef CAMERA_H
#define CAMERA_H

#include "Vector2z.h"
#include <gtk/gtk.h>

struct Camera{
    Camera();
    Vector2z pos;
    Vector2z viewport;
    Vector2z size;
    Vector2z clip;
    float angle;
    float yu, yd, xl, xr;  // limites da viewport (depende do clipping)
    bool clip_method;
    Vector2z world_to_norm(Vector2z coord);
    Vector2z norm_to_view(Vector2z coord);
    Vector2z world_to_viewport(Vector2z coord);
    int get_rcode(Vector2z point);
    std::vector<Vector2z> cohen_sutherland_clipper(Vector2z point1, Vector2z point2);
    std::vector<Vector2z> liang_barsky_clipper(Vector2z point0, Vector2z point1);
    void clip_draw_line(cairo_t* cr, std::vector<Vector2z> points);
    void clip_draw_point(cairo_t* cr, Vector2z point);
    void clip_draw_polygon(cairo_t* cr, std::vector<Vector2z> points, gboolean filled);
    void clip_pol_aux(std::vector<Vector2z>& new_polygon, Vector2z e1, Vector2z e2);
    Vector2z intersection(Vector2z e1, Vector2z e2, Vector2z a, Vector2z b);
    void clip_draw_curve(cairo_t* cr, std::vector<Vector2z> points);
};
#endif
