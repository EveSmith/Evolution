#include "Server.h"
#include <algorithm>
#include <iostream>



Server::Server(int WIDTH, int HEIGHT, int INIT_SIZE){
	E = new Environment(WIDTH, HEIGHT);
	width = WIDTH;
	height = HEIGHT;
	for (int i = 0; i < INIT_SIZE; i++) {
		addOrg();
	}
}

Server::~Server(){
	delete E;
	for (auto it : ORG_LIST) {
		delete it.second;
	}
}

std::string Server::printEnviron(bool grid){
	if (grid){
		return E->printGrid();
	}
	else{
		return E->printList();
	}
}

std::string Server::printOrgList() {
	std::string list = "Organism List:\n";
	int x = ORG_LIST.size();
	for (auto it : ORG_LIST) {
		list.append(it.second->print() + "\n");
	}
	return list;
}

std::string Server::printThoughts() {
	std::string toReturn = "";
	for (auto it : thoughtRelevance) {
		toReturn.append("Situation: "+it.first.situation.subject + "," + it.first.situation.trait + "," + it.first.situation.value + "\n");
		toReturn.append("Action: " + it.first.action + "\n");
		toReturn.append("Rating: " + std::to_string(it.first.rating)+"\n");
		toReturn.append("\tTimes thought: " + std::to_string(it.second)+"\n\n");
	}
	return toReturn;
}



void Server::org_update() {
	for (auto it : ORG_LIST) {
		it.second->updateSelf();
		it.second->sendUpdate(SERVER_INBOX);
	}
}


void Server::server_update(){
	OrgUpdate currUpdate;
	ServerUpdate servUpdate;
	//For all the messages in inbox...
	while (!SERVER_INBOX.empty()){
		//Deal with first message
		currUpdate = SERVER_INBOX.front();
		//If Org is marked for death, kill it
		if (!currUpdate.alive){
			killOrg(currUpdate.senderID);
			SERVER_INBOX.pop();
			continue;
		}

		else if (currUpdate.action == "Eat") {
			E->changeResources(currUpdate.oldX, currUpdate.oldY, "Food", currUpdate.amount);
			ORG_LIST[currUpdate.senderID]->healing(currUpdate.amount);
		}
		else if (currUpdate.action == "Attack") {
			ORG_LIST[currUpdate.targetID]->injury(currUpdate.amount);
		}
		else if (currUpdate.action == "MatingOn") {
			mateable[currUpdate.senderID] = { currUpdate.newX, currUpdate.newY };
		}
		else if (currUpdate.action == "MatingOff") {
			mateable.erase(currUpdate.senderID);
		}
		else if (currUpdate.action == "Move") {
			E->moveOrg(currUpdate.senderID, currUpdate.oldX, currUpdate.oldY, currUpdate.newX, currUpdate.newY);
			if (mateable.count(currUpdate.senderID) > 0) {
				mateable[currUpdate.senderID] = { currUpdate.newX, currUpdate.newY };
			}
		}



		//Remove message from inbox
		SERVER_INBOX.pop();


		//Compile server update
		servUpdate.newX = currUpdate.newX;
		servUpdate.newY = currUpdate.newY;
		servUpdate.checked = false;
		servUpdate.surroundings = compileSurroundings(currUpdate.senderID, currUpdate.newX, currUpdate.newY);
		//Org list is a map, with IDs as keys
		ORG_LIST[currUpdate.senderID]->receiveUpdate(servUpdate);
	}

	std::vector<int> alreadyMated;
	//For every org with mating toggled on...
	for (auto parent1 : mateable) {
		//Look at every other org with mating toggled on...
		for (auto parent2 : mateable) {
			//Check if either has already mated
			auto p1 = std::find(alreadyMated.begin(), alreadyMated.end(), parent1.first);
			auto p2 = std::find(alreadyMated.begin(), alreadyMated.end(), parent2.first);
			//If neither org has already mated, their positions are the same, and they aren't the same org...
			if (p1 == alreadyMated.end() && p2 == alreadyMated.end() && parent1.second == parent2.second && parent1.first != parent2.first) {
				//If neither parent is newborn...
				if (!ORG_LIST[parent1.first]->getTraits().Newborn && !ORG_LIST[parent2.first]->getTraits().Newborn) {
					//Create a new organism nearby that has their combined DNA.
					int randtemp = rand() % 4;
					std::array<int, 2> childpos = parent1.second;
					if (randtemp == 0 && childpos[0] != width-1) { childpos[0]++; }
					else if (randtemp == 1 && childpos[0] != 0) { childpos[0]--; }
					else if (randtemp == 2 && childpos[1] != height-1) { childpos[1]++; }
					else if (randtemp == 3 && childpos[1] != 0) { childpos[1]--; }
					addOrg(childpos[0],childpos[1], recombineDNA(ORG_LIST[parent1.first]->getDNA(), ORG_LIST[parent2.first]->getDNA()));
					alreadyMated.push_back(parent1.first);
					alreadyMated.push_back(parent2.first);
				}
			}
		}
	}
}


std::string Server::recombineDNA(std::string parent1, std::string parent2) {
	std::string childDNA = "";
	int mutation_chance = 10000;
	int mutationType = rand() % 3;
	int i = 0;
	try {
		while(true) {
			if (rand() % 2 == 0) { //MIGHT DECREASE DNA LENGTH OVER TIME
				childDNA += parent1.at(i);
			}
			else {
				childDNA += parent2.at(i);
			}
			//Mutation
			if (rand() % mutation_chance == 0) {
				if (mutationType == 0) { //Addition
					childDNA.append(std::to_string(rand() % 2));
				}
				else if (mutationType == 1) { //Deletion
					childDNA.pop_back();
				}
				else { //Alteration
					childDNA.replace(i, 1, std::to_string(rand() % 2));
				}
			}
			i++;
		}
	}
	catch(std::out_of_range e){

	}
	return childDNA;
}


//CURRENTLY ONLY ALLOWS SENSING IN STRAIGHT LINE FROM CURRENT LOCATION
//DOES NOT INCLUDE ANY CORNERS
//IS ALSO EXTREMELY MESSY!! FIX LATER!!
Surroundings Server::compileSurroundings(int ID, int x, int y) {
	Surroundings toReturn;
	std::array<int, 2> positionTemp = { 0,0 };

	//Compile surrounding food amounts
	toReturn.foodNearby.push_back(std::make_pair(positionTemp, E->getResources(x, y, "Food")));
	if (x != width - 1) { positionTemp = { 1,0 }; toReturn.foodNearby.push_back(std::make_pair(positionTemp, E->getResources(x + 1, y, "Food"))); }
	if(y!=height-1){ positionTemp = { 0,1 }; toReturn.foodNearby.push_back(std::make_pair(positionTemp, E->getResources(x, y+1, "Food"))); }
	if(x!=0) { positionTemp = { -1,0 }; toReturn.foodNearby.push_back(std::make_pair(positionTemp, E->getResources(x-1, y, "Food"))); }
	if(y!=0) { positionTemp = { 0,-1 }; toReturn.foodNearby.push_back(std::make_pair(positionTemp, E->getResources(x, y-1, "Food"))); }

	//Compile surrounding organisms
	std::vector<int> currentOrgList = E->getOrgs(x, y);
	OrgSense currentOrgSense;
	positionTemp = { 0,0 };
	Traits currentOrgTraits;
	for (int i = 0; i < currentOrgList.size(); i++) {
		currentOrgTraits = ORG_LIST[currentOrgList[i]]->getTraits();
		currentOrgSense.ID = currentOrgList[i];
		currentOrgSense.color = currentOrgTraits.Color;
		currentOrgSense.size = currentOrgTraits.Size;
		currentOrgSense.mateable = currentOrgTraits.Mateable;
		currentOrgSense.newborn = currentOrgTraits.Newborn;
		toReturn.orgsNearby.push_back(std::make_pair(positionTemp, currentOrgSense));
	}
	if (x != width - 1) {
		currentOrgList = E->getOrgs(x + 1, y);
		positionTemp = { 1,0 };
		for (int i = 0; i < currentOrgList.size(); i++) {
			currentOrgTraits = ORG_LIST[currentOrgList[i]]->getTraits();
			currentOrgSense.ID = currentOrgList[i];
			currentOrgSense.color = currentOrgTraits.Color;
			currentOrgSense.size = currentOrgTraits.Size;
			currentOrgSense.mateable = currentOrgTraits.Mateable;
			currentOrgSense.newborn = currentOrgTraits.Newborn;
			toReturn.orgsNearby.push_back(std::make_pair(positionTemp, currentOrgSense));
		}
	}
	if (y != height - 1) {
		currentOrgList = E->getOrgs(x, y+1);
		positionTemp = { 0,1 };
		for (int i = 0; i < currentOrgList.size(); i++) {
			currentOrgTraits = ORG_LIST[currentOrgList[i]]->getTraits();
			currentOrgSense.ID = currentOrgList[i];
			currentOrgSense.color = currentOrgTraits.Color;
			currentOrgSense.size = currentOrgTraits.Size;
			currentOrgSense.mateable = currentOrgTraits.Mateable;
			currentOrgSense.newborn = currentOrgTraits.Newborn;
			toReturn.orgsNearby.push_back(std::make_pair(positionTemp, currentOrgSense));
		}
	}
	if (x != 0) {
		currentOrgList = E->getOrgs(x-1, y);
		positionTemp = { -1,0 };
		for (int i = 0; i < currentOrgList.size(); i++) {
			currentOrgTraits = ORG_LIST[currentOrgList[i]]->getTraits();
			currentOrgSense.ID = currentOrgList[i];
			currentOrgSense.color = currentOrgTraits.Color;
			currentOrgSense.size = currentOrgTraits.Size;
			currentOrgSense.mateable = currentOrgTraits.Mateable;
			currentOrgSense.newborn = currentOrgTraits.Newborn;
			toReturn.orgsNearby.push_back(std::make_pair(positionTemp, currentOrgSense));
		}
	}
	if (y != 0) {
		currentOrgList = E->getOrgs(x, y-1);
		positionTemp = { 0,-1 };
		for (int i = 0; i < currentOrgList.size(); i++) {
			currentOrgTraits = ORG_LIST[currentOrgList[i]]->getTraits();
			currentOrgSense.ID = currentOrgList[i];
			currentOrgSense.color = currentOrgTraits.Color;
			currentOrgSense.size = currentOrgTraits.Size;
			currentOrgSense.mateable = currentOrgTraits.Mateable;
			currentOrgSense.newborn = currentOrgTraits.Newborn;
			toReturn.orgsNearby.push_back(std::make_pair(positionTemp, currentOrgSense));
		}
	}

	return toReturn;
}


void Server::addOrg(){
	Organism* org = new Organism(width, height);
	ORG_LIST[org->getID()] = org;
	E->addOrg(org->getID(), org->getX(), org->getY());
	ORG_LIST[org->getID()]->setSurroundings(compileSurroundings(org->getID(), org->getX(), org->getY()));
}

void Server::addOrg(int initX, int initY, std::string initDNA) {
	Organism* org = new Organism(width, height, initX, initY, initDNA);
	ORG_LIST[org->getID()] = org;
	E->addOrg(org->getID(), initX, initY);
	ORG_LIST[org->getID()]->setSurroundings(compileSurroundings(org->getID(), initX, initY));
}


void Server::killOrg(int id){
	mateable.erase(id);
	E->remOrg(id);
	delete ORG_LIST[id];
	ORG_LIST.erase(id);
}