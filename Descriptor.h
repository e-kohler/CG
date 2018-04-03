#ifndef DESCRIPTOR_H
#define DESCRIPTOR_H

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace Descriptor{

	extern std::vector<std::string> lines;


	void importObject(char* filename);
	void translator();
	void exportObject();

	const std::vector<std::string> split(const std::string &s, char c);
		

}

#endif