#ifndef COORD_H
#define COORD_H

class Coord {  // essa classe é só um vetor 2d
    public:
        Coord();
        Coord (float x, float y);
        float getX();
        float getY();
        void setX(float x);
        void setY(float y);
        Coord operator+(Coord coord);
        Coord operator-(Coord coord); 
        Coord operator*(float number);
        Coord operator/(float number);
            
    private:
        float x;
        float y;
};

#endif