#include "Camera.h"
#include <math.h>
#include <iostream>
#define PI 3.14159265

Camera::Camera() {
    pos = Vector2z(0, 0);
    viewport = Vector2z(480, 480);  // propriedades da camera que limita o mundo
    size = Vector2z(10, 10);
    angle = 0;
    clip = Vector2z(1.6, 1.6);
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
    float yu = clip.getY()/2;
    float yd = -clip.getY()/2;
    float xl = -clip.getX()/2;  // determinando os limites do clip
    float xr = clip.getX()/2;

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

void Camera::draw_clipped (Clipped to_be_clipped, cairo_t* cr) {
    auto coord1_normed = world_to_norm(to_be_clipped.coord1);
    auto coord2_normed = world_to_norm(to_be_clipped.coord2);

    auto clipped = clip_line(coord1_normed, coord2_normed);

    if (clipped.draw) {

        auto coord1_clipped_view = norm_to_view(clipped.coord1);
        auto coord2_clipped_view = norm_to_view(clipped.coord2);
  
        cairo_move_to(cr, coord1_clipped_view.getX(), coord1_clipped_view.getY());
        cairo_line_to(cr, coord2_clipped_view.getX(), coord2_clipped_view.getY());
        cairo_stroke(cr);
    }
}

Clipped Camera::clip_line(Vector2z point0, Vector2z point1) {
    float yu = clip.getY()/2;
    float yd = -clip.getY()/2;
    float xl = -clip.getX()/2;  // determinando os limites do clip
    float xr = clip.getX()/2;

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
	if (accept) {
		return Clipped(Vector2z(x0, y0), Vector2z(x1, y1));
	}
}