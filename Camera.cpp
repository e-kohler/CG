#include "Camera.h"
#include "Trans.h"
#include <math.h>

Camera::Camera() {
    pos = Vector2z(0, 0);
    viewport = Vector2z(480, 480);  // propriedades da camera que limita o mundo
    size = Vector2z(20, 20);
    angle = 0;
}

Vector2z Camera::world_to_viewport(Vector2z coord) {
    std::vector<std::vector<float> > matrix;

    float cos_ang  = cos(-angle);
    float sin_ang = sin(-angle);
    float size_x = 2 / size.getX();
    float size_y = 2 / size.getY();
    float pos_x = pos.getX();
    float pos_y = pos.getY();
    float view_x = viewport.getX() / 2;
    float view_y = -viewport.getY() / 2;

    matrix = {  {cos_ang * size_x, -sin_ang * size_y, 0},   // translada, rotaciona e escalona
                {sin_ang * size_x, cos_ang * size_y,  0},   // transforma em coordenada normalizada
                {(pos_x * -cos_ang + pos_y * sin_ang) * size_x, (sin_ang * pos_x - pos_y * cos_ang) * size_y, 1}};

    coord = coord * matrix;

    matrix = {  {view_x,  0,      0},  // translada e escalona de volta
                {0,       view_y, 0},  // transforma em coordenada de viewport
                {view_x, -view_y, 1}
    };

    coord = coord * matrix;

    return coord;



    /**Vector2z wmin = pos - (size/2);
    Vector2z output = coord - wmin;
    output = Vector2z(output.getX()/size.getX(), 1 - (output.getY()/size.getY()));  // isso é aquelas contas dos slides
    output = Vector2z(output.getX() * viewport.getX(), output.getY() * viewport.getY());
    return output;**/
}