#include "Descriptor.h"
#include <iostream>

void Descriptor::importObject(char* filename){
	std::ifstream infile(filename);

	for (std::string line; getline(infile, line); ){
		 
	}
}

void Descriptor::exportObject(){

}