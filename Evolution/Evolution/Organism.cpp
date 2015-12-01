#include "Organism.h"
#include <iostream>

static int UNIVERSAL_ID = 0;

Organism::Organism(){
	//Unique ID for each organism, hopefully.
	this->ID = UNIVERSAL_ID;
	UNIVERSAL_ID++;

	this->health = 100;

	//Generate some length of DNA (currently length 1)
	this->DNA = "";
	for (int i = 0; i < 1; i++){
		this->DNA.append(std::to_string(rand() % 10));
	}

	//set org position to 0,0
	this->position.fill(0);

	this->pendingUpdate = { true, 0, 0 };
}

Organism::Organism(int initX, int initY){
	//Unique ID for each organism, hopefully.
	this->ID = UNIVERSAL_ID;
	UNIVERSAL_ID++;

	this->health = 100;

	//Generate some length of DNA (currently length 1)
	this->DNA = "";
	for (int i = 0; i < 1; i++){
		this->DNA.append(std::to_string(rand() % 10));
	}

	//set org position to 0,0
	this->position[0] = initX;
	this->position[1] = initY;

	this->pendingUpdate = { true, 0, 0 };
}

Organism::Organism(std::string str){
	this->ID = UNIVERSAL_ID;
	UNIVERSAL_ID++;

	this->health = 100;

	//set DNA
	this->DNA = str;

	//set org position to 0,0
	this->position.fill(0);

	this->pendingUpdate = { true, 0, 0 };
}

std::string Organism::print(){
	std::string info = "Organism: "+std::to_string(ID)+"\n\tPosition: " +
		std::to_string(position[0]) + "," + std::to_string(position[1]) + "\n"+
		"\tHealth: " + std::to_string(health) + "\n"
		"\tDNA: " + DNA + "\n";
	return info;
}


int Organism::getID(){
	return this->ID;
}

int Organism::getX(){
	return this->position[0];
}

int Organism::getY(){
	return this->position[1];
}


void Organism::receiveUpdate(ServerUpdate update){
	this->pendingUpdate = update;
}

void Organism::checkUpdates(){
	std::cout << "Organism is checking for updates.\n";
	if (!this->pendingUpdate.checked){
		this->position[0] = this->pendingUpdate.newX;
		this->position[1] = this->pendingUpdate.newY;
	}
	this->pendingUpdate.checked = true;
	std::cout << "Organism is done checking for updates.\n";
}

void Organism::adjustHealth(int delta){
	this->health += delta;
}


//CHANGE HARD CODED PARTS!!!!
void Organism::move(){
	int direction = rand() % 4;
	if (direction == 0 && this->position[0]!=4){ this->position[0]++; }
	else if (direction = 1 && this->position[1]!=4){ this->position[1]++; }
	else if (direction = 2 && this->position[0] != 0){ this->position[0]--; }
	else if (direction = 3 && this->position[1] != 0){ this->position[1]--; }
}

//Decide on action, send update to server
void Organism::sendUpdate(std::queue<OrgUpdate> &inbox){
	OrgUpdate update;
	update.oldX = this->position[0];
	update.oldY = this->position[1];

	this->move();

	update.senderID = this->ID;
	update.newX = this->position[0];
	update.newY = this->position[1];

	std::cout << "Org " + std::to_string(this->ID) + " is accessing server inbox.\n";
	inbox.push(update);
	std::cout << "Org " + std::to_string(this->ID) + " has finished accessing server inbox.\n";
}
