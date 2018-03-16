#include "Coord.h"

Coord::Coord() {}

Coord::Coord(float x, float y) {
    this->x = x;
    this->y = y;
}

float Coord::getX() {
    return x;
}

float Coord::getY() {
    return y;
}

void Coord::setX(float x) {
    this->x = x;
}

void Coord::setY(float y) {
    this->y = y;
}

Coord Coord::operator+(Coord coord) {
    return Coord(this->x + coord.getX(), this->y + coord.getY());
}

Coord Coord::operator-(Coord coord) {
    return Coord(this->x - coord.getX(), this->y - coord.getY());
}

Coord Coord::operator*(float number) {
    return Coord(this->x * number, this->y * number);
}

Coord Coord::operator/(float number) {
    return Coord(this->x / number, this->y / number);
}

