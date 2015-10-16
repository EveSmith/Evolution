#include "Environment.h"

#include <random>


Environment::Environment(int WIDTH, int HEIGHT){
	width = WIDTH;
	height = HEIGHT;
	size = width*height;

	//matrix = new Cell[size];

	//will give same environment every time!!
	float yOffset = 0.0;
	//Increment perlin noise
	float increment = 0.02;
	//Multiplier for Cell value
	float maxValue = 10;

	//For each spot in the matrix...
	for (int y = 0; y < height; y++){
		yOffset += increment;
		float xOffset = 0.0;
		for (int x = 0; x < width; x++){
			xOffset += increment;
			//generate new Cell with specific value.
			//CURRENTLY GENERATES RANDOMLY, WILL CHANGE TO 
			//PERLIN NOISE GENERATION LATER
			matrix.push_back(new Cell(rand() % int(maxValue)));
		}
	}
}

Environment::~Environment(){
	for (int i = 0; i < matrix.size; i++){
		delete matrix[i];
	}
}


std::string Environment::print(){
	std::string info;
	for (int i = 0; i < size; i++){
		int col = (i % width);
		int row = (i - col) / height;
		info.append("(" + std::to_string(col) + "," + std::to_string(row) + "):\n");
		info.append(matrix[i]->printInfo()+"\n\n");
	}
	return info;
}

