#include "Clipped.h"

Clipped::Clipped() : draw(false) {}

Clipped::Clipped(Vector2z coord1, Vector2z coord2) : coord1(coord1), coord2(coord2), draw(true) {}