#include "Descriptor.h"

Descriptor::importObject(){
	ofstream myfile;

	myfile.open ("example.txt");
	

}

Descriptor::exportObject(){
	ofstream myfile;

	myfile << "Writing this to a file.\n";
	myfile.close();
}