#ifndef COORD_H
#define COORD_H

class Coord {
    public:
        Coord();
        Coord (float x, float y);
        float getX();
        float getY();
        Coord operator+(Coord coord);
        Coord operator-(Coord coord); 
        Coord operator*(float number);
        Coord operator/(float number);
            
    private:
        float x;
        float y;
};

#endif