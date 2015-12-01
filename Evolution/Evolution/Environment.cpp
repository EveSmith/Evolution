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
			matrix.push_back(new Cell());
		}
	}
}

Environment::~Environment(){
	for (int i = 0; i < matrix.size(); i++){
		delete matrix[i];
	}
}


std::string Environment::printList(){
	std::string info;
	for (int i = 0; i < size; i++){
		int col = (i % width);
		int row = (i - col) / height;
		info.append("(" + std::to_string(col) + "," + std::to_string(row) + "):\n");
		info.append(matrix[i]->printInfoList()+"\n\n");
	}
	return info;
}

std::string Environment::printGrid(){
	std::string grid;
	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			grid.append("+---");
		}
		grid.append("+\n");
		for (int j = 0; j < width; j++){
			grid.append("|"+matrix[(i*width)+j]->printInfoGrid());
		}
		grid.append("|\n");
	}
	for (int j = 0; j < width; j++){
		grid.append("+---");
	}
	grid.append("+\n");
	return grid;
}

//Returns matrix index of specified organism (not in coordinate form)
//VERY SLOW!!! OPTIMIZE LATER!!!
int Environment::findOrg(int id){
	for (int i = 0; i < matrix.size(); i++){
		if (matrix[i]->containsOrg(id)){
			return i;
		}
	}
	return -1;
}



//Put the ID of the organism into the specified Cell
void Environment::addOrg(int id, int x, int y){
	matrix[y*width + x]->insertOrg(id);
}

//Find and remove organism
void Environment::remOrg(int id){
	int orgIndex = findOrg(id);
	if (orgIndex != -1){
		matrix[orgIndex]->removeOrg(id);
	}
}

//Remove organism from specified location
void Environment::remOrg(int id, int x, int y){
	matrix[y*width + x]->removeOrg(id);
}


//Relocates organism
void Environment::moveOrg(int id, int oldX, int oldY, int newX, int newY){
	this->remOrg(id);
	this->addOrg(id, newX, newY);
}