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

	for (auto partner1 : mateable) {
		std::vector<int> alreadyMated;
		for (auto partner2 : mateable) {
			if (partner1.second == partner2.second) { //neither org has already mated, neither are newborn, and positions are same...
				//add new org at that location that combines their dna.
			}
		}
	}
}

//Check validity of requested action, resolve conflicts
void Server::confirmUpdate(OrgUpdate &currUpdate, ServerUpdate &servUpdate){
	//Fix invalid movement
	if (currUpdate.newX >= width){ currUpdate.newX = width - 1; }
	if (currUpdate.newX < 0) { currUpdate.newX = 0; }
	if (currUpdate.newY >= height){ currUpdate.newY = height - 1; }
	if (currUpdate.newY < 0) { currUpdate.newY = 0; }
	servUpdate.newX = currUpdate.newX;
	servUpdate.newY = currUpdate.newY;
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
	E->remOrg(id);
	delete ORG_LIST[id];
	ORG_LIST.erase(id);
}