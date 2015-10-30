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

	//set org position to 0,0
	position.fill(0);
}

Organism::Organism(int initX, int initY){
	//Unique ID for each organism, hopefully.
	ID = UNIVERSAL_ID;
	UNIVERSAL_ID++;

	health = 100;

	//Generate some length of DNA (currently length 1)
	DNA = "";
	for (int i = 0; i < 1; i++){
		DNA.append(std::to_string(rand() % 10));
	}

	//set org position to 0,0
	position[0] = initX;
	position[1] = initY;
}

Organism::Organism(std::string str){
	ID = UNIVERSAL_ID;
	UNIVERSAL_ID++;

	health = 100;

	//set DNA
	DNA = str;

	//set org position to 0,0
	position.fill(0);
}

std::string Organism::print(){
	std::string info = "Organism: "+std::to_string(ID)+"\n\tPosition: " +
		std::to_string(position[0]) + "," + std::to_string(position[1]) + "\n"+
		"\tHealth: " + std::to_string(health) + "\n"
		"\tDNA: " + DNA + "\n";
	return info;
}


int Organism::getID(){
	return ID;
}

int Organism::getX(){
	return position[0];
}

int Organism::getY(){
	return position[1];
}


//NOTHING FOR NOW BECAUSE ORGS ARE BLIND RANDOM WALKERS
void Organism::receiveUpdate(ServerUpdate update){
	position[0] = update.newX;
	position[1] = update.newY;
	if ((update.cellValue - std::stoi(DNA)) == 0){
		adjustHealth(3);
	}
	else if ((update.cellValue - std::stoi(DNA)) > 3 || (update.cellValue - std::stoi(DNA)) < -3){
		adjustHealth(-3);
	}
}


void Organism::adjustHealth(int delta){
	health += delta;
}


//CHANGE HARD CODED PARTS!!!!
void Organism::move(){
	int direction = rand() % 4;
	if (direction == 0 && position[0]!=4){ position[0]++; }
	else if (direction = 1 && position[1]!=4){ position[1]++; }
	else if (direction = 2 && position[0] != 0){ position[0]--; }
	else if (direction = 3 && position[1] != 0){ position[1]--; }
}

//Decide on action, send update to server
void Organism::update(std::queue<OrgUpdate> &inbox){
	OrgUpdate update;
	update.oldX = position[0];
	update.oldY = position[1];

	this->move();

	update.senderID = ID;
	update.newX = position[0];
	update.newY = position[1];

	inbox.push(update);
}
