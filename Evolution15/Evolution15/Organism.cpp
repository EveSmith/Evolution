#include "Organism.h"
#include <iostream>

static int UNIVERSAL_ID = 0;

/*
Organism handles:
	Health -- Server does not need to know health of organisms
*/

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
	newborn = true;

	std::vector<OrgSense> dummyVector;
	CellSense dummyCellSense = { 0,0,0, dummyVector };
	surroundings.push_back(dummyCellSense);
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
	newborn = true;

	std::vector<OrgSense> dummyVector;
	CellSense dummyCellSense = { 0,0,0, dummyVector };
	surroundings.push_back(dummyCellSense);
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
	newborn = true;

	std::vector<OrgSense> dummyVector;
	CellSense dummyCellSense = { 0,0,0, dummyVector };
	surroundings.push_back(dummyCellSense);
}

std::string Organism::print(){
	std::string info = "Organism: " + std::to_string(ID) + "\n\tPosition: " +
		std::to_string(position[0]) + "," + std::to_string(position[1]) + "\n" +
		"\tHealth: " + std::to_string(health) + "\n" +
		"\tDNA: " + DNA + "\n" +
		"\tResources on position: " + std::to_string(surroundings[0].food) + "\n";
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
	checkUpdates();
}

void Organism::checkUpdates(){
	if (!pendingServerUpdate.checked){
		position[0] = pendingServerUpdate.newX;
		position[1] = pendingServerUpdate.newY;
		surroundings = pendingServerUpdate.surroundings;
	}
	pendingServerUpdate.checked = true;
}

void Organism::injureSelf(int amount){
	health -= amount;
	if (health < 0) { health = 0; }
}

void Organism::healSelf(int amount){
	health += amount;
	if (health > 100) { health = 100; }
}


//Send update to server
void Organism::sendUpdate(std::queue<OrgUpdate> &inbox){
	inbox.push(pendingOrgUpdate);
	pendingOrgUpdate.eatrequest.request_made = false;
	pendingOrgUpdate.attackrequest.request_made = false;
	pendingOrgUpdate.materequest.request_made = false;
	pendingOrgUpdate.healrequest.request_made = false;
}


void Organism::updateSelf(){
	pendingOrgUpdate.senderID = ID;
	health -= 4;

	if (health <= 0) {
		pendingOrgUpdate.alive = false;
	}

	pendingOrgUpdate.oldX = position[0];
	pendingOrgUpdate.oldY = position[1];

	if (!newborn) {
		reason();

		while (!actionPlan.empty()) {
			actionPlan.front()();
			actionPlan.pop();
		}
	}
	else {
		move(0, 0);
		newborn = false;
	}
	pendingOrgUpdate.sensoryrequest = { perception };

	std::cout << std::endl;
}


//TO DO: MAKE BEHAVIOR RELY ON SURROUNDINGS
void Organism::reason(){
	bool mated = false;
	for (int i = 0; i < forethought; i++) {
		//If there's more orgs on current space...
		//(NOT 100% FUNCTIONAL IF MORE THAN 2 ORGS ON SAME SPACE!! FIX!!)
		if (surroundings[0].orgs.size()>0 && rand()%101<25) {
			if(!mated){
				std::function<void()> mate_lambda = [=]() {mate(surroundings[0].orgs[0].ID); };
				actionPlan.push(mate_lambda);
				mated = true;
				std::cout << "Organism " + std::to_string(ID) + " tried to mate with Organism " + std::to_string(surroundings[0].orgs[0].ID) + "." << std::endl;
			}
			else if (rand()%2 == 0) {
				std::function<void()> attack_lambda = [=]() {attack(surroundings[0].orgs[0].ID); };
				actionPlan.push(attack_lambda);
				std::cout << "Organism " + std::to_string(ID) + " attacked Organism " + std::to_string(surroundings[0].orgs[0].ID) + "." << std::endl;
			}
			else {
				std::function<void()> heal_lambda = [=]() {heal(surroundings[0].orgs[0].ID); };
				actionPlan.push(heal_lambda);
				std::cout << "Organism " + std::to_string(ID) + " transferred some health to Organism "+ std::to_string(surroundings[0].orgs[0].ID) + "."<< std::endl;
			}
		}
		else {
			if (rand()%101<health) {
				int maxfoodIndex = 0;
				for (int j = 0; j < surroundings.size(); j++) {
					if (surroundings[j].food > surroundings[maxfoodIndex].food || rand()%101<25) {
						maxfoodIndex = j;
					}
					else if (surroundings[j].food == surroundings[maxfoodIndex].food) {
						if (rand() % 2 == 0) {
							maxfoodIndex = j;
						}
					}
				}
				std::function<void()> move_lambda = [=]() { move(surroundings[maxfoodIndex].deltaX, surroundings[maxfoodIndex].deltaY); };
				actionPlan.push(move_lambda);
				if (maxfoodIndex == 0) {
					std::cout << "Organism " + std::to_string(ID) + " stayed still." << std::endl;
				}
				else {
					std::cout << "Organism " + std::to_string(ID) + " moved." << std::endl;
				}
			}
			else {
				std::function<void()> eat_lambda = [=]() {eat(); };
				actionPlan.push(eat_lambda);
				std::cout << "Organism " + std::to_string(ID) + " ate." << std::endl;
			}
			
		}
		
	}
}

void Organism::move(int deltaX, int deltaY){
	pendingOrgUpdate.newX = this->position[0]+deltaX;
	pendingOrgUpdate.newY = this->position[1]+deltaY;
}

void Organism::eat(){
	pendingOrgUpdate.eatrequest.request_made = true;
	pendingOrgUpdate.eatrequest.amount = 5;
}

void Organism::mate(int mateID){
	pendingOrgUpdate.materequest.request_made = true;
	pendingOrgUpdate.materequest.mateID = mateID;
}

void Organism::attack(int victimID){
	pendingOrgUpdate.attackrequest.request_made = true;
	pendingOrgUpdate.attackrequest.victimID = victimID;
	pendingOrgUpdate.attackrequest.strength = 10;
}

void Organism::heal(int friendID) {
	pendingOrgUpdate.healrequest.request_made = true;
	pendingOrgUpdate.healrequest.friendID = friendID;
	pendingOrgUpdate.healrequest.strength = 10;
}