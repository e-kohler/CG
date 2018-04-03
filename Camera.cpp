#include "Camera.h"
#include <math.h>
#include <iostream>
#define PI 3.14159265

Camera::Camera() {
    pos = Vector2z(0, 0);
    viewport = Vector2z(480, 480);  // propriedades da camera que limita o mundo
    size = Vector2z(20, 20);
    angle = 0;
}

Vector2z Camera::world_to_viewport(Vector2z coord) {
    std::vector<std::vector<float> > matrix;

    float cos_ang  = cos(angle);
    float sin_ang = sin(angle);
    float size_x = 2 / size.getX();
    float size_y = 2 / size.getY();
    float pos_x = pos.getX();
    float pos_y = pos.getY();
    float view_x = viewport.getX() / 2;
    float view_y = -viewport.getY() / 2;

    matrix = {  {cos_ang * size_x, -sin_ang * size_y, 0},   // translada, rotaciona e escalona
                {sin_ang * size_x, cos_ang * size_y,  0},   // transforma em coordenada normalizada
                {(pos_x * -cos_ang - pos_y * sin_ang) * size_x, (sin_ang * pos_x - pos_y * cos_ang) * size_y, 1}};

    coord = coord * matrix;

    matrix = {  {view_x,  0,      0},  // translada e escalona de volta
                {0,       view_y, 0},  // transforma em coordenada de viewport
                {view_x, -view_y, 1}
    };

    coord = coord * matrix;

    return coord;
}

int Camera::get_rcode(Vector2z point) {
    float yu = pos.getY() + size.getY()/2;
    float yd = pos.getY() - size.getY()/2;
    float xl = pos.getX() - size.getX()/2;  // determinando os limites do clip
    float xr = pos.getX() + size.getX()/2;

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

std::vector<Vector2z> Camera::clip_line(Vector2z point1, Vector2z point2) {
    int rcode1 = get_rcode(point1);
    int rcode2 = get_rcode(point2);

    std::vector<Vector2z> clipped_coords;

    if (rcode1 == rcode2 == 0) {  // está totalmente dentro da window
        clipped_coords = {point1, point2};
        return clipped_coords;
    }

    if ((rcode1 & rcode2) != 0) {  // está totalmente fora da window
        clipped_coords = {Vector2z(0, 0), Vector2z(0, 0)};
        return clipped_coords;
    }
}

Vector2z Camera::clip_point(Vector2z point, float m) {
    int rcode = get_rcode(point);
    
    Vector2z clipped_point;
    
    float yu = pos.getY() + size.getY()/2;
    float yd = pos.getY() - size.getY()/2;
    float xl = pos.getX() - size.getX()/2;  // determinando os limites do clip
    float xr = pos.getX() + size.getX()/2;

    if (!rcode) {
        return point;
    }

    if (rcode & 1) {
        float y = m * (xl - point.getX()) + point.getY();  // esquerda
        if (y < yu && y > yd) {
            clipped_point = Vector2z(xl, y);
        }
    }

    if (rcode & 2) {
        float y = m * (xr - point.getX()) + point.getY();  // direita
        if (y < yu && y > yd) {
            clipped_point = Vector2z(xr, y);
        }
    }

    if (rcode & 4) {
        float x = point.getX() + 1/m * (yd - point.getY());  // baixo
        if (x < xr && x > xl) {
            clipped_point = Vector2z(x, yd);
        }
    }

    if (rcode & 8) {
        float x = point.getX() + 1/m * (yu - point.getY()); 
        if (x < xr && x > xl) {
            clipped_point = Vector2z(x, yu);
        }
    }
    return clipped_point;
}
