#include "Vector2z.h"

Vector2z::Vector2z() {}

Vector2z::Vector2z(float x, float y) : coords(3) {
    this->coords[0] = x;
    this->coords[1] = y;
    this->coords[2] = 1;
}

float Vector2z::getX() {
    return this->coords[0];
}

float Vector2z::getY() {
    return this->coords[1];
}

std::vector<float> Vector2z::getCoords() {
    return this->coords;
}

void Vector2z::setX(float x) {
    this->coords[0] = x;
}

void Vector2z::setY(float y) {
    this->coords[1] = y;
}

Vector2z Vector2z::operator+(Vector2z vector) {
    return Vector2z(this->coords[0] + vector.getX(), this->coords[1] + vector.getY());
}

Vector2z Vector2z::operator-(Vector2z vector) {
    return Vector2z(this->coords[0] - vector.getX(), this->coords[1] - vector.getY());
}

Vector2z Vector2z::operator*(float number) {
    return Vector2z(this->coords[0] * number, this->coords[1] * number);
}

Vector2z Vector2z::operator/(float number) {
    return Vector2z(this->coords[0] / number, this->coords[1] / number);
}

