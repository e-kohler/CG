#include "Camera.h"
#include <math.h>
#include <iostream>
#include <algorithm>

#define PI 3.14159265

Camera::Camera() {
    pos = Vector2z(0, 0);
    viewport = Vector2z(480, 480);  // propriedades da camera que limita o mundo
    size = Vector2z(10, 10);
    angle = 0;
    clip = Vector2z(1.6, 1.6);
    clip_method = false;  // true: cohen-sutherland, false: liang-barsky
    yu = clip.getY()/2;
    yd = -clip.getY()/2;
    xl = -clip.getX()/2;
    xr = clip.getX()/2;
}

Vector2z Camera::world_to_norm(Vector2z coord) {
    std::vector<std::vector<float> > matrix;

    float cos_ang  = cos(angle);
    float sin_ang = sin(angle);
    float size_x = 2 / size.getX();
    float size_y = 2 / size.getY();
    float pos_x = pos.getX();
    float pos_y = pos.getY();

    matrix = {  {cos_ang * size_x, -sin_ang * size_y, 0},   // translada, rotaciona e escalona
                {sin_ang * size_x, cos_ang * size_y,  0},   // transforma em coordenada normalizada
                {(pos_x * -cos_ang - pos_y * sin_ang) * size_x, (sin_ang * pos_x - pos_y * cos_ang) * size_y, 1}};

    coord = coord * matrix;
    return coord;
}

Vector2z Camera::norm_to_view(Vector2z coord) {
    std::vector<std::vector<float> > matrix;

    float view_x = viewport.getX() / 2;
    float view_y = -viewport.getY() / 2;

    matrix = {  {view_x,  0,      0},  // translada e escalona de volta
                {0,       view_y, 0},  // transforma em coordenada de viewport
                {view_x, -view_y, 1}
    };

    coord = coord * matrix;

    return coord;
}

Vector2z Camera::world_to_viewport(Vector2z coord) {
    auto norm = world_to_norm(coord);

    return norm_to_view(norm);
}

int Camera::get_rcode(Vector2z point) {

    float x = point.getX();
    float y = point.getY();

    int reg_code = 0;  // cada ponto tem um codigo, (CIMA, BAIXO, DIREITA, ESQUERDA), sendo cada 1 ou 0.

    if (x < xl)
         reg_code |= 1;

    if (x > xr)
         reg_code |= 2;

    if (y < yd)
         reg_code |= 4;

    if (y > yu)
         reg_code |= 8;

    return reg_code;
}

void Camera::clip_draw_line (cairo_t* cr, std::vector<Vector2z> points) {
    auto point1_norm = world_to_norm(points[0]);
    auto point2_norm = world_to_norm(points[1]);

    std::vector<Vector2z> clipped;

    if (clip_method) {
        clipped = cohen_sutherland_clipper(point1_norm, point2_norm);
        std::cout << "usando cohen suth" << std::endl;
    } else {
        clipped = liang_barsky_clipper(point1_norm, point2_norm);
        std::cout << "usando liang barsky" << std::endl;
    }

    if (clipped.size() > 0) {

        auto coord1_clipped_view = norm_to_view(clipped[0]);
        auto coord2_clipped_view = norm_to_view(clipped[1]);

        cairo_move_to(cr, coord1_clipped_view.getX(), coord1_clipped_view.getY());
        cairo_line_to(cr, coord2_clipped_view.getX(), coord2_clipped_view.getY());
        cairo_stroke(cr);
    }
}

std::vector<Vector2z> Camera::cohen_sutherland_clipper(Vector2z point0, Vector2z point1) {
    float x0 = point0.getX();
    float y0 = point0.getY();
    float x1 = point1.getX();
    float y1 = point1.getY();

    int outcode0 = get_rcode(Vector2z(x0, y0));
    int outcode1 = get_rcode(Vector2z(x1, y1));
    bool accept = false;

    while (true) {
        if (!(outcode0 | outcode1)) {
            accept = true;
            break;
        } else if (outcode0 & outcode1) {
            return std::vector<Vector2z>{};
            break;
        } else {
            float x, y;

            int outcodeOut = outcode0 ? outcode0 : outcode1;

            if (outcodeOut & 8) {
                x = x0 + (x1 - x0) * (yu - y0) / (y1 - y0);
                y = yu;
            } else if (outcodeOut & 4) {
                x = x0 + (x1 - x0) * (yd - y0) / (y1 - y0);
                y = yd;
            } else if (outcodeOut & 2) {
                y = y0 + (y1 - y0) * (xr - x0) / (x1 - x0);
                x = xr;
            } else if (outcodeOut & 1) {
                y = y0 + (y1 - y0) * (xl - x0) / (x1 - x0);
                x = xl;
            }

            if (outcodeOut == outcode0) {
                x0 = x;
                y0 = y;
                outcode0 = get_rcode(Vector2z(x0, y0));
            } else {
                x1 = x;
                y1 = y;
                outcode1 = get_rcode(Vector2z(x1, y1));
            }
        }
    }
    return std::vector<Vector2z>{Vector2z(x0, y0), Vector2z(x1, y1)};
}

std::vector<Vector2z> Camera::liang_barsky_clipper(Vector2z point0, Vector2z point1) {
    float p1 = -(point1.getX() - point0.getX());
    float p2 = -p1;
    float p3 = -(point1.getY() - point0.getY());
    float p4 = -p3;

    float q1 = point0.getX() - xl;
    float q2 = xr - point0.getX();
    float q3 = point0.getY() - yd;
    float q4 = yu - point0.getY();

    float posarr[5], negarr[5];
    int posind = 1, negind = 1;
    posarr[0] = 1;
    negarr[0] = 0;

    if ((p1 == 0 && q1 < 0) || (p3 == 0 && q3 < 0)) {
        return std::vector<Vector2z>{};
    }
    if (p1 != 0) {
        float r1 = q1 / p1;
        float r2 = q2 / p2;
        if (p1 < 0) {
        negarr[negind++] = r1;
        posarr[posind++] = r2;
        } else {
        negarr[negind++] = r2;
        posarr[posind++] = r1;
        }
    }
    if (p3 != 0) {
        float r3 = q3 / p3;
        float r4 = q4 / p4;
        if (p3 < 0) {
        negarr[negind++] = r3;
        posarr[posind++] = r4;
        } else {
        negarr[negind++] = r4;
        posarr[posind++] = r3;
        }
    }

    float xn1, yn1, xn2, yn2;
    float rn1, rn2;
    rn1 = *std::max_element(negarr, negarr + negind);
    rn2 = *std::min_element(posarr, posarr + posind);

    if (rn1 > rn2)  {
        return std::vector<Vector2z>{};
    }

    xn1 = point0.getX() + p2 * rn1;
    yn1 = point0.getY() + p4 * rn1;

    xn2 = point0.getX() + p2 * rn2;
    yn2 = point0.getY() + p4 * rn2;

    return std::vector<Vector2z>{Vector2z(xn1, yn1), Vector2z(xn2, yn2)};
}

void Camera::clip_draw_point(cairo_t* cr, Vector2z point) {

    point = world_to_norm(point);

    if ((point.getX() < xl || point.getX() > xr || point.getY() < yd || point.getY() > yu)) {
        return;
    }

    point = norm_to_view(point);
    cairo_move_to(cr, point.getX(), point.getY());
    cairo_arc(cr, point.getX(), point.getY(), 1, 0, 2*M_PI);
    cairo_stroke(cr);
}

Vector2z Camera::intersection(Vector2z e1, Vector2z e2, Vector2z a, Vector2z b) {
    double x = (e1.getX()*e2.getY() - e1.getY()*e2.getX()) * (a.getX() - b.getX()) - (e1.getX() - e2.getX()) * (a.getX()*b.getY() - a.getY()*b.getX());
    x /= (e1.getX() - e2.getX()) * (a.getY() - b.getY()) - (e1.getY() - e2.getY()) * (a.getX() - b.getX());

    double y = (e1.getX()*e2.getY() - e1.getY()*e2.getX()) * (a.getY() - b.getY()) - (e1.getY() - e2.getY()) * (a.getX()*b.getY() - a.getY()*b.getX());
    y /= (e1.getX() - e2.getX()) * (a.getY() - b.getY()) - (e1.getY() - e2.getY()) * (a.getX() - b.getX());

    return Vector2z(x, y);
}

void Camera::clip_pol_aux(std::vector<Vector2z>& new_polygon, Vector2z e1, Vector2z e2) {
    std::vector<Vector2z> new_points{};

    for (auto i = 0u; i < new_polygon.size(); ++i) {
        auto k = (i+1)%new_polygon.size();

        Vector2z a = Vector2z(new_polygon[i]);
        Vector2z b = Vector2z(new_polygon[k]);

        double a_pos = (e2.getX()-e1.getX()) * (a.getY()-e1.getY()) - (e2.getY()-e1.getY()) * (a.getX()-e1.getX());
        double b_pos = (e2.getX()-e1.getX()) * (b.getY()-e1.getY()) - (e2.getY()-e1.getY()) * (b.getX()-e1.getX());

        if (a_pos >= 0 && b_pos >= 0) { // se os dois pontos tao dentro
            new_points.push_back(a);
        }
        else if (a_pos < 0 && b_pos >= 0) { // se só o 'a' estiver fora
            new_points.push_back(intersection(e1, e2, a, b));
        }
        else if (a_pos >= 0 && b_pos < 0) { // se só o 'b' estiver fora
            new_points.push_back(a);
            new_points.push_back(intersection(e1, e2, a, b));
        }
    }

    if (new_points.size() == 0) {
        new_polygon.clear();
        new_polygon.push_back(Vector2z(0,0));
    } else {
        new_polygon = new_points;
    }
}

void Camera::clip_draw_polygon(cairo_t* cr, std::vector<Vector2z> points, gboolean filled) {
    std::vector<Vector2z> clip_window{
        Vector2z(xl, yd),
        Vector2z(xr, yd),
        Vector2z(xr, yu),
        Vector2z(xl, yu)
    };

    std::vector<Vector2z> new_polygon{points};

    for (int i = 0; i < points.size(); i++) {
        new_polygon[i] = world_to_norm(new_polygon[i]);
    }

    for (auto i = 0u; i < clip_window.size(); ++i) {
        auto k = (i+1)%clip_window.size();
        clip_pol_aux(new_polygon, clip_window[i], clip_window[k]);
    }

    if (new_polygon.size() > 0) {
        auto va = norm_to_view(new_polygon[0]);
        cairo_move_to(cr, va.getX(), va.getY());
        for (auto i = 1u; i < new_polygon.size(); ++i) {
            auto vb = norm_to_view(new_polygon[i]);

            cairo_line_to(cr, vb.getX(), vb.getY());
        }
        cairo_line_to(cr, va.getX(), va.getY());

        if (filled) {
            cairo_fill(cr);
        } else {
            cairo_stroke(cr);
        }
    }
}

void Camera::clip_draw_curve(cairo_t* cr, std::vector<Vector2z> points) {
    for (auto i = 0u; i < points.size()-1; ++i) {

      auto point_a = points[i];
      auto point_b = points[i+1];

      clip_draw_line(cr, std::vector<Vector2z>{point_a, point_b});
  }
}
