#include "Edge.h"

Edge::Edge(Vector a, Vector b) {
	this->pointA = a;
	this->pointB = b;
}

Edge::~Edge() {}

Vector Edge::getA() {
	return this->pointA;
}

Vector Edge::getB() {
	return this->pointB;
}