#ifndef EDGE_H
#define EDGE_H

#include "Vector.h"

class Edge {
    protected:
        Vector pointA;
        Vector pointB;
           
    public:
        Edge(Vector a, Vector b);
        ~Edge();
        Vector getA();
        Vector getB();
};

#endif