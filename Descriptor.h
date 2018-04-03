#ifndef DESCRIPTOR_H
#define DESCRIPTOR_H

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include "Shape.h"

namespace Descriptor{

	extern std::vector<std::string> lines;


	void importObject(char* filename);
	std::list<Shape*> translator();
	void exportObject();

	const std::vector<std::string> split(const std::string &s, char c);
		

}

#endif