#ifndef COORD_H
#define COORD_H

class Coord {
    public:
        Coord (float x, float y);
        float getX();
        float getY();
    
    private:
        float x;
        float y;
};

#endif