#include "Descriptor.h"
#include <iostream>

void Descriptor::importObject(){
	std::string line;
	std::ifstream infile("teste.obj");

	for (std::string line; getline(infile, line); ){
			 std::cout << line << std::endl;
	}
}

void Descriptor::exportObject(){
	
}