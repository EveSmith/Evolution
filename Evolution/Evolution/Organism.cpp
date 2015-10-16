#include "Organism.h"

int UNIVERSAL_ID = 0;

Organism::Organism(){
	//Unique ID for each organism, hopefully.
	ID = UNIVERSAL_ID;
	UNIVERSAL_ID++;

	health = 100;

	//Generate some length of DNA (currently length 1)
	DNA = "";
	for (int i = 0; i < 1; i++){
		DNA.append(std::to_string(rand() % 10));
	}
	
	//goal value is the DNA number generated (will change)
	goalValue = int(DNA[0]);

	//set org position to 0,0
	position.fill(0);
}

Organism::Organism(std::string str){
	ID = UNIVERSAL_ID;
	UNIVERSAL_ID++;

	health = 100;

	//set DNA
	DNA = str;

	//goal value is the DNA number generated (will change)
	goalValue = int(DNA[0]);

	//set org position to 0,0
	position.fill(0);
}

std::string Organism::print(){
	std::string info = "Organism:\n\tPosition: " +
		std::to_string(position[0]) + "," + std::to_string(position[1]) + "\n"+
		"\tHealth: " + std::to_string(health) + "\n"
		"\tGoal Value: " + std::to_string(goalValue) + "\n";
	return info;
}

void Organism::adjustHealth(int delta){
	health += delta;
}

void Organism::move(){
	int direction = rand() % 4;
	if (direction == 0){ position[0]++; }
	else if (direction = 1){ position[1]++; }
	else if (direction = 2){ position[0]--; }
	else{ position[1]--; }
	
	
}

