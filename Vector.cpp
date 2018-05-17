#include "Vector.h"

std::vector<float> Vector::getCoords(){
    return this->coords;
}

Vector::Vector(){}

Vector::Vector(float x, float y) : coords(3) {
    this->coords[0] = x;
    this->coords[1] = y;
    this->coords[2] = 1;
}

Vector::Vector(float x, float y, float z) : coords(4) {
    this->coords[0] = x;
    this->coords[1] = y;
    this->coords[2] = z;
    this->coords[3] = 1;
} 

float Vector::getX() {
    return this->coords[0];
}

float Vector::getY() {
    return this->coords[1];
}


void Vector::setX(float x) {
    this->coords[0] = x;
}

void Vector::setY(float y) {
    this->coords[1] = y;
}

Vector2z::Vector2z(){}

Vector2z::Vector2z(float x, float y) : Vector(x, y) {}

Vector2z Vector2z::operator+(Vector2z vector) {
    return Vector2z(this->coords[0] + vector.getX(), this->coords[1] + vector.getY());
}

Vector2z Vector2z::operator-(Vector2z vector) {
    return Vector2z(this->coords[0] - vector.getX(), this->coords[1] - vector.getY());
}

bool Vector2z::operator==(Vector2z vector) {
    if ((this->coords[0] == vector.getX()) && (this->coords[1] == vector.getY())) {
        return true;
    } else {
        return false;
    }
}

Vector2z Vector2z::operator*(float number) {
    return Vector2z(this->coords[0] * number, this->coords[1] * number);
}

Vector2z Vector2z::operator*(std::vector<std::vector<float> > matrix) {
    std::vector<float> result(3);
    for (int i = 0; i < 3; i++) {
        result[i] = 0;
        for (int j = 0; j < 3; j++) {
            result[i] += coords[j] * matrix[j][i];
        }
    }
    return Vector2z(result[0], result[1]);
}

Vector2z Vector2z::operator/(float number) {
    return Vector2z(this->coords[0] / number, this->coords[1] / number);
}

Vector3z::Vector3z(){}

Vector3z::Vector3z(float x, float y, float z) : Vector(x, y, z) {}

float Vector3z::getZ() {
    return this->coords[2];
}

void Vector3z::setZ(float z) {
    this->coords[2] = z;
}

Vector3z Vector3z::operator+(Vector3z vector) {
    return Vector3z(this->coords[0] + vector.getX(), this->coords[1] + vector.getY(), this->coords[2] + vector.getZ());
}

Vector3z Vector3z::operator-(Vector3z vector) {
    return Vector3z(this->coords[0] - vector.getX(), this->coords[1] - vector.getY(), this->coords[2] + vector.getZ());
}

bool Vector3z::operator==(Vector3z vector) {
    if ((this->coords[0] == vector.getX()) && (this->coords[1] == vector.getY()) && ((this->coords[2] == vector.getY()))) {
        return true;
    } else {
        return false;
    }
}

Vector3z Vector3z::operator*(float number) {
    return Vector3z(this->coords[0] * number, this->coords[1] * number, this->coords[2]*number);
}

Vector3z Vector3z::operator*(std::vector<std::vector<float> > matrix) {
    std::vector<float> result(4);
    for (int i = 0; i < 4; i++) {
        result[i] = 0;
        for (int j = 0; j < 4; j++) {
            result[i] += coords[j] * matrix[j][i];
        }
    }
    return Vector3z(result[0], result[1], result[2]);
}

Vector3z Vector3z::operator/(float number) {
    return Vector3z(this->coords[0] / number, this->coords[1] / number, this->coords[2]/ number);
}



