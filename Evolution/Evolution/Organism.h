#ifndef ORGANISM_H
#define ORGANISM_H

#include <string>
#include <array>

class Organism{
public:
	//Generates DNA, spawns organism from the void
	Organism();
	Organism(std::string str);

	std::string print();

	void adjustHealth(int delta);
	void move();

private:
	int ID;
	int health; //org dies when health reaches 0
	std::string DNA;
	int goalValue; //goal value that org wants to be at

	std::array<int, 2> position;
	int currentSpaceValue;
	
};


#endif //ORGANISM_H