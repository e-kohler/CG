#ifndef VECTOR2Z_H
#define VECTOR2Z_H

#include <vector>

class Vector2z {  // essa classe é só um vetor 2d
    public:
        Vector2z();
        Vector2z (float x, float y);
        float getX();
        float getY();
        std::vector<float> getCoords();
        void setX(float x);
        void setY(float y);
        Vector2z operator+(Vector2z vector);
        Vector2z operator-(Vector2z vector); 
        Vector2z operator*(float number);
        Vector2z operator*(std::vector<std::vector<float> > matrix);
        Vector2z operator/(float number);
            
    private:
        std::vector<float> coords;
};

#endif