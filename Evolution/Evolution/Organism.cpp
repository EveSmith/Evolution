#include "Organism.h"
#include <iostream>

static int UNIVERSAL_ID = 0;

Organism::Organism(){
	//Unique ID for each organism, hopefully.
	this->ID = UNIVERSAL_ID;
	UNIVERSAL_ID++;

	this->health = 100;

	//Generate some length of DNA (currently length 1)
	DNA = "";
	for (int i = 0; i < 1; i++){
		DNA.append(std::to_string(rand() % 10));
	}

	//set org position to 0,0
	position.fill(0);

	pendingServerUpdate = { true, 0, 0 };
}

Organism::Organism(int initX, int initY){
	//Unique ID for each organism, hopefully.
	this->ID = UNIVERSAL_ID;
	UNIVERSAL_ID++;

	this->health = 100;

	//Generate some length of DNA (currently length 1)
	DNA = "";
	for (int i = 0; i < 1; i++){
		DNA.append(std::to_string(rand() % 10));
	}

	//set org position to 0,0
	position[0] = initX;
	position[1] = initY;

	pendingServerUpdate = { true, 0, 0 };
}

Organism::Organism(std::string str){
	this->ID = UNIVERSAL_ID;
	UNIVERSAL_ID++;

	this->health = 100;

	//set DNA
	DNA = str;

	//set org position to 0,0
	position.fill(0);

	pendingServerUpdate = { true, 0, 0 };
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


void Organism::receiveUpdate(ServerUpdate update){
	this->pendingServerUpdate = update;
}

void Organism::checkUpdates(){
	if (!pendingServerUpdate.checked){
		position[0] = pendingServerUpdate.newX;
		position[1] = pendingServerUpdate.newY;
	}
	pendingServerUpdate.checked = true;
}

void Organism::adjustHealth(int delta){
	health += delta;
}


//Decide on action, send update to server
void Organism::sendUpdate(std::queue<OrgUpdate> &inbox){
	inbox.push(pendingOrgUpdate);
}

void Organism::updateSelf(){
	pendingOrgUpdate.senderID = ID;

	pendingOrgUpdate.oldX = position[0];
	pendingOrgUpdate.oldY = position[1];

	reason();

	while (!actionPlan.empty()){
		if (actionPlan.front() == "Move"){move(); actionPlan.pop();
		std::cout << "Organism " << ID << " moved!" << std::endl;
		}
		else if (actionPlan.front() == "Eat"){ eat(); actionPlan.pop();
		std::cout << "Organism " << ID << " ate!" << std::endl;
		}
		else if (actionPlan.front() == "Mate"){ mate(0); actionPlan.pop();
		std::cout << "Organism " << ID << " tried to mate!" << std::endl;
		}
		else if (actionPlan.front() == "Attack"){ attack(0); actionPlan.pop();
		std::cout << "Organism " << ID << " tried to attack!" << std::endl;
		}
	}
	std::cout << std::endl;

	pendingOrgUpdate.health = health;
}


//Currently completely blind and random.
void Organism::reason(){
	for (int i = 0; i < forethought; i++){
		int action = rand() % 4;
		if (action == 0){ actionPlan.push("Move"); }
		else if (action == 1){ actionPlan.push("Eat"); }
		else if (action == 2){ actionPlan.push("Mate"); }
		else if (action == 3){ actionPlan.push("Attack"); }
	}
}

void Organism::move(){
	int direction = rand() % 5;
	if (direction == 0 && position[0] != 4){ position[0]++; }
	else if (direction = 1 && position[1] != 4){ position[1]++; }
	else if (direction = 2 && position[0] != 0){ position[0]--; }
	else if (direction = 3 && position[1] != 0){ position[1]--; }
	else{};

	pendingOrgUpdate.newX = position[0];
	pendingOrgUpdate.newY = position[1];
}

void Organism::eat(){

}

void Organism::mate(int mateID){

}

void Organism::attack(int victimID){

}