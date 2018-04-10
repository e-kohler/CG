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
    clip_method = true;  // true: cohen-sutherland, false: liang-barsky
    clip_yu = clip.getY()/2;
    clip_yd = -clip.getY()/2;
    clip_xl = -clip.getX()/2;  // determinando os limites do clip
    clip_xr = clip.getX()/2;
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

    if (x < clip_xl)
         reg_code |= 1;
 
    if (x > clip_xr)
         reg_code |= 2;
 
    if (y < clip_yd)
         reg_code |= 4;
 
    if (y > clip_yu)
         reg_code |= 8;
    
    return reg_code;
}

void Camera::draw_clipped (Clipped to_be_clipped, cairo_t* cr) {
    auto coord1_normed = world_to_norm(to_be_clipped.coord1);
    auto coord2_normed = world_to_norm(to_be_clipped.coord2);

    Clipped clipped;
    
    if (clip_method)
        clipped = cohen_sutherland_clipper(coord1_normed, coord2_normed);
    else
        clipped = liang_barsky_clipper(coord1_normed, coord2_normed);

    if (clipped.draw) {

        auto coord1_clipped_view = norm_to_view(clipped.coord1);
        auto coord2_clipped_view = norm_to_view(clipped.coord2);
  
        cairo_move_to(cr, coord1_clipped_view.getX(), coord1_clipped_view.getY());
        cairo_line_to(cr, coord2_clipped_view.getX(), coord2_clipped_view.getY());
        cairo_stroke(cr);
    }
}

Clipped Camera::cohen_sutherland_clipper(Vector2z point0, Vector2z point1) {
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
            return Clipped();
			break;
		} else {
			float x, y;

			int outcodeOut = outcode0 ? outcode0 : outcode1;

			if (outcodeOut & 8) {
				x = x0 + (x1 - x0) * (clip_yu - y0) / (y1 - y0);
				y = clip_yu;
			} else if (outcodeOut & 4) {
				x = x0 + (x1 - x0) * (clip_yd - y0) / (y1 - y0);
				y = clip_yd;
			} else if (outcodeOut & 2) {
				y = y0 + (y1 - y0) * (clip_xr - x0) / (x1 - x0);
				x = clip_xr;
			} else if (outcodeOut & 1) {
				y = y0 + (y1 - y0) * (clip_xl - x0) / (x1 - x0);
				x = clip_xl;
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
	if (accept) {
        std::cout << "usando cohen-sutherland" << std::endl;
		return Clipped(Vector2z(x0, y0), Vector2z(x1, y1));
	}
}

Clipped Camera::liang_barsky_clipper(Vector2z point0, Vector2z point1) {
    float p1 = -(point1.getX() - point0.getX());
    float p2 = -p1;
    float p3 = -(point1.getY() - point0.getY());
    float p4 = -p3;

    float q1 = point0.getX() - clip_xl;
    float q2 = clip_xr - point0.getX();
    float q3 = point0.getY() - clip_yd;
    float q4 = clip_yu - point0.getY();

    float posarr[5], negarr[5];
    int posind = 1, negind = 1;
    posarr[0] = 1;
    negarr[0] = 0;

    if ((p1 == 0 && q1 < 0) || (p2 == 0 && q2 < 0) || (p3 == 0 && q3 < 0) || (p4 == 0 && q4 < 0)) {
        return Clipped();
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
        return Clipped();
    }

    xn1 = point0.getX() + p2 * rn1;
    yn1 = point0.getY() + p4 * rn1;

    xn2 = point0.getX() + p2 * rn2;
    yn2 = point0.getY() + p4 * rn2;

    std::cout << "usando liang-barsky" << std::endl;
    return Clipped(Vector2z(xn1, yn1), Vector2z(xn2, yn2));
}

void Camera::clip_and_draw_point(Vector2z point, cairo_t* cr) {
    point = world_to_norm(point);

    if ((point.getX() < clip_xl || point.getX() > clip_xr || point.getY() < clip_yd || point.getY() > clip_yu)) {
        return;
    }

    point = norm_to_view(point);
    cairo_move_to(cr, point.getX(), point.getY());
    cairo_arc(cr, point.getX(), point.getY(), 1, 0, 2*M_PI);
    cairo_stroke(cr);
}

float x_intersect(float x1, float y1, float x2, float y2,
                float x3, float y3, float x4, float y4) {
    float num = (x1*y2 - y1*x2) * (x3-x4) -
              (x1-x2) * (x3*y4 - y3*x4);
    float den = (x1-x2) * (y3-y4) - (y1-y2) * (x3-x4);
    return num/den;
}
 
float y_intersect(float x1, float y1, float x2, float y2,
                float x3, float y3, float x4, float y4) {
    float num = (x1*y2 - y1*x2) * (y3-y4) -
              (y1-y2) * (x3*y4 - y3*x4);
    float den = (x1-x2) * (y3-y4) - (y1-y2) * (x3-x4);
    return num/den;
}
 
// This functions clips all the edges w.r.t one clip
// edge of clipping area
void poly_clipper(std::list<Vector2z> coords, float x1, float y1, float x2, float y2) {
    
    std::list<Vector2z> new_points;
 
    // (ix,iy),(kx,ky) are the co-ordinate values of
    // the points
    for (auto it = coords.begin(); it != coords.end(); ++it) {
        Vector2z i = *it;

        auto it2 = ++it;

        Vector2z k = *(it2);
        if (it == prev(coords.end()))
            k = *coords.begin();

        float i_pos = (x2 - x1) * (i.getY() - y1) - (y2 - y1) * (i.getX() - x1);
        float k_pos = (x2 - x1) * (k.getY() - y1) - (y2 - y1) * (k.getX() - x1);

        if (i_pos < 0 && k_pos < 0) {
            new_points.push_back(k);

        } else if (i_pos >= 0 && k_pos < 0) {

            float inter_xi = x_intersect(x1, y1, x2, y2, i.getX(), i.getY(), k.getX(), k.getY());
            float inter_yi = y_intersect(x1, y1, x2, y2, i.getX(), i.getY(), k.getX(), k.getY());
            new_points.push_back(Vector2z(inter_xi, inter_yi));
            new_points.push_back(k);

        } else if (i_pos < 0 && k_pos >= 0) {
            float inter_xi = x_intersect(x1, y1, x2, y2, i.getX(), i.getY(), k.getX(), k.getY());
            float inter_yi = y_intersect(x1, y1, x2, y2, i.getX(), i.getY(), k.getX(), k.getY());
            new_points.push_back(Vector2z(inter_xi, inter_yi));

        } else {

        }
    }
    coords = new_points;
}

// Implements Sutherland–Hodgman algorithm
void sutherland_hodgeman_clipper(std::list<Vector2z> poly_coords) {
    /*for (int i=0; i<clipper_size; i++)
    {
        int k = (i+1) % clipper_size;
 
        // We pass the current array of vertices, it's size
        // and the end points of the selected clipper line
        clip(poly_points, poly_size, clipper_points[i][0],
             clipper_points[i][1], clipper_points[k][0],
             clipper_points[k][1]);
    }
 
    // Printing vertices of clipped polygon
    for (int i=0; i < poly_size; i++)
        cout << '(' << poly_points[i][0] <<
                ", " << poly_points[i][1] << ") ";*/
}