#include "View.h"

View::View() {
    pos = Coord(0, 0);
    viewport = Coord(400, 400);  // propriedades da camera que limita o mundo
    size = Coord(10, 10);
}

Coord View::world_to_viewport(Coord coord) {
    Coord wmin = pos - (size/2);
    Coord output = coord - wmin;
    output = Coord(output.getX()/size.getX(), 1 - (output.getY()/size.getY()));  // isso é aquelas contas dos slides
    output = Coord(output.getX() * viewport.getX(), output.getY() * viewport.getY());
    return output;
}