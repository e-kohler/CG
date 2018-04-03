#include "Descriptor.h"
#include <iostream>
#include <cstring>
#include "Vector2z.h"
#include <sstream>
#include <list>
#include "Shape.h"

std::vector<std::string> Descriptor::lines;

void Descriptor::importObject(char* filename){
	std::ifstream infile(filename);
	
	std::cout<< "imported" << filename << std::endl;
	for (std::string line; getline(infile, line); ){
		 lines.push_back(line);
	}

	Descriptor::translator();
}

void Descriptor::translator(){
	for(auto it = lines.begin(); it != lines.end(); ++it) {
		
		auto line = *it;
		auto inst = line.front();

		auto pieces = split(line, ' ');

		std::list<Shape>* shapes;
		std::list<Vector2z> vertices;

		switch(inst){
			case 'v':
				vertices.push_back(Vector2z(std::stof(pieces[1]), std::stof(pieces[2])));
				break;
			case 'o':
				break;
			case 'f':
				break;
			case 'l':
				break;
		}

	}
}

void Descriptor::exportObject(){

}

const std::vector<std::string> Descriptor::split(const std::string &s, char c) {
    std::string buff {""};
    std::vector<std::string> v;
    for (auto n:s) {
        if (n!= c) buff+=n; else
        if (n == c && buff != "") {v.push_back(buff); buff="";}
    }
    if (buff != "") v.push_back(buff);
    return v;
}