#ifndef VIEW_H
#define CAMERA_H

#include "Coord.h"

struct View{
    View();
    Coord pos;
    Coord viewport;
    Coord size;
    Coord world_to_viewport(Coord coord);
};
#endif