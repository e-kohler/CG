#include "Camera.h"

Camera::Camera() {
    pos = Vector2z(0, 0);
    viewport = Vector2z(480, 480);  // propriedades da camera que limita o mundo
    size = Vector2z(35, 35);
    angle = 0;
}

Vector2z Camera::world_to_viewport(Vector2z coord) {
    Vector2z wmin = pos - (size/2);
    Vector2z output = coord - wmin;
    output = Vector2z(output.getX()/size.getX(), 1 - (output.getY()/size.getY()));  // isso é aquelas contas dos slides
    output = Vector2z(output.getX() * viewport.getX(), output.getY() * viewport.getY());
    return output;
}