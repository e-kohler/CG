#ifndef VECTOR_H
#define VECTOR_H
#include <vector>

class Vector {
  protected:
        std::vector<float> coords;

  public:
        Vector();
        Vector(float x, float y);
        Vector(float x, float y, float z);
        std::vector<float> getCoords();
        float getX();
        float getY();
        void setX(float x);
        void setY(float y);
};

class Vector2z : public Vector {  // essa classe é só um vetor 2d
    public:
        Vector2z();
        Vector2z (float x, float y);

        Vector2z operator+(Vector2z vector);
        Vector2z operator-(Vector2z vector);
        bool operator==(Vector2z vector);
        Vector2z operator*(float number);
        Vector2z operator*(std::vector<std::vector<float> > matrix);
        Vector2z operator/(float number);
};

class Vector3z : public Vector {  // essa classe é só um vetor 3d
    public:
        Vector3z();
        Vector3z (float x, float y, float z);
        float getZ();
        void setZ(float z);      

        Vector3z operator+(Vector3z vector);
        Vector3z operator-(Vector3z vector);
        bool operator==(Vector3z vector);
        Vector3z operator*(float number);
        Vector3z operator*(std::vector<std::vector<float> > matrix);
        Vector3z operator/(float number); 
};





#endif