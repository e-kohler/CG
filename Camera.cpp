#include "Camera.h"
#include <math.h>
#include <iostream>
#define PI 3.14159265

Camera::Camera() {
    pos = Vector2z(0, 0);
    viewport = Vector2z(480, 480);  // propriedades da camera que limita o mundo
    size = Vector2z(20, 20);
    angle = 0;
    clip = Vector2z(0.5, 0.5);
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

    reg_code |= (x < xl) << 0;
    reg_code |= (x > xr) << 1;
    reg_code |= (y < yd) << 2;
    reg_code |= (y > yu) << 3;
    
    return reg_code;
}

void Camera::draw_clipped (Clipped clipped, cairo_t* cr) {
    std::cout << "world" << std::endl;
    std::cout << clipped.coord1.getX() << "  " << clipped.coord1.getY() << std::endl;
    std::cout << clipped.coord2.getX() << "  " << clipped.coord2.getY() << std::endl;
    clipped.coord1 = world_to_norm(clipped.coord1);
    clipped.coord2 = world_to_norm(clipped.coord2);
    std::cout << "norm" << std::endl;
    std::cout << clipped.coord1.getX() << "  " << clipped.coord1.getY() << std::endl;
    std::cout << clipped.coord2.getX() << "  " << clipped.coord2.getY() << std::endl;

    clipped = clip_line(clipped.coord1, clipped.coord2);

    if (clipped.draw) {

        clipped.coord1 = norm_to_view(clipped.coord1);
        clipped.coord2 = norm_to_view(clipped.coord2);
  
        cairo_move_to(cr, clipped.coord1.getX(), clipped.coord2.getY());
        cairo_line_to(cr, clipped.coord2.getX(), clipped.coord2.getY());
        cairo_stroke(cr);
    }
}

Clipped Camera::clip_line(Vector2z point1, Vector2z point2) {
    int rcode1 = get_rcode(point1);
    int rcode2 = get_rcode(point2);

    std::cout << "region code" << std::endl;
    std::cout << rcode1 << std::endl;
    std::cout << rcode2 << std::endl;

    if (!rcode1 && !rcode2) {  // está totalmente dentro da window
        return Clipped(point1, point2);
    }

    if (rcode1 & rcode2) {  // está totalmente fora da window
        return Clipped();// nao desenha
    }

    float m = (point1.getY() - point2.getY()) / (point1.getX() - point2.getX());

    auto clipped_coord1 = clip_line_point(point1, m);
    auto clipped_coord2 = clip_line_point(point2, m);

    std::cout << "clipped" << std::endl;
    std::cout << clipped_coord1.getX() << "  " << clipped_coord1.getY() << std::endl;
    std::cout << clipped_coord2.getX() << "  " << clipped_coord2.getY() << std::endl;

    if (clipped_coord1 == point1 && clipped_coord2 == point2) {
        return Clipped();
    } 
    else {
        return Clipped(clipped_coord1, clipped_coord2);
    }
}

Vector2z Camera::clip_line_point(Vector2z point, float m) {
    int rcode = get_rcode(point);
    
    Vector2z clipped_point;
    
    float yu = clip.getY()/2; 
    float yd = -clip.getY()/2; 
    float xl = -clip.getX()/2;  // determinando os limites do clip
    float xr = clip.getX()/2;

    bool clipped = false;

    if (!rcode) {  // se code for 0000, já esta dentro
        return point;
    }

    if (rcode & (1 << 0)) {
        float y = m * (xl - point.getX()) + point.getY();  // esquerda
        if (y < yu && y > yd) {
            clipped_point = Vector2z(xl, y);
            clipped = true;
        }
    }

    if (rcode & (1 << 1)) {
        float y = m * (xr - point.getX()) + point.getY();  // direita
        if (y < yu && y > yd) {
            clipped_point = Vector2z(xr, y);
            clipped = true;
        }
    }

    if (rcode & (1 << 2)) {
        float x = point.getX() + 1/m * (yd - point.getY());  // baixo
        if (x < xr && x > xl) {
            clipped_point = Vector2z(x, yd);
            clipped = true;
        }
    }

    if (rcode & (1 << 3)) {
        float x = point.getX() + 1/m * (yu - point.getY()); 
        if (x < xr && x > xl) {
            clipped_point = Vector2z(x, yu);
            clipped = true;
        }
    }

    if (!clipped) {
        return point;
    }

    return clipped_point;
}
