#include "Server.h"
#include <algorithm>
#include <iostream>


Server::Server(int WIDTH, int HEIGHT, std::vector<Organism*>* OFFICIAL_ORG_LIST){
	E = new Environment(WIDTH, HEIGHT);
	width = WIDTH;
	height = HEIGHT;
	official_org_list = OFFICIAL_ORG_LIST;
}

Server::~Server(){
	delete E;
}

std::string Server::printEnviron(bool grid){
	if (grid){
		return E->printGrid();
	}
	else{
		return E->printList();
	}
}


void Server::update(std::queue<OrgUpdate> &inbox){
	OrgUpdate currUpdate;
	ServerUpdate servUpdate;
	//For all the messages in inbox...
	while (!inbox.empty()){
		//Deal with first message
		currUpdate = inbox.front();
		//If Org is marked for death, kill it
		if (!currUpdate.alive){
			killOrg(currUpdate.senderID);
		}

		else if (currUpdate.action == "Eat") {
			E->changeResources(currUpdate.oldX, currUpdate.oldY, "Food", currUpdate.amount);
			orgList[currUpdate.senderID]->healing(currUpdate.amount);
		}
		else if (currUpdate.action == "Attack") {
			orgList[currUpdate.targetID]->injury(currUpdate.amount);
		}
		else if (currUpdate.action == "ToggleMating") {

		}
		else if (currUpdate.action == "Move") {
			E->moveOrg(currUpdate.senderID, currUpdate.oldX, currUpdate.oldY, currUpdate.newX, currUpdate.newY);
		}
		//confirmUpdate(currUpdate, servUpdate); //Remove and roll into orgs??
		//Move Org from old position to new position



		//Remove message from inbox
		inbox.pop();


		//Compile server update
		servUpdate.newX = currUpdate.newX;
		servUpdate.newY = currUpdate.newY;
		servUpdate.checked = false;
		servUpdate.surroundings = compileSurroundings(currUpdate.senderID, currUpdate.newX, currUpdate.newY);
		//Org list is a map, with IDs as keys
		orgList[currUpdate.senderID]->receiveUpdate(servUpdate);
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
	for (int i = 0; i < currentOrgList.size(); i++) {
		currentOrgSense.ID = currentOrgList[i];
		toReturn.orgsNearby.push_back(std::make_pair(positionTemp, currentOrgSense));
	}
	if (x != width - 1) {
		std::vector<int> currentOrgList = E->getOrgs(x + 1, y);
		OrgSense currentOrgSense;
		positionTemp = { 1,0 };
		for (int i = 0; i < currentOrgList.size(); i++) {
			currentOrgSense.ID = currentOrgList[i];
			toReturn.orgsNearby.push_back(std::make_pair(positionTemp, currentOrgSense));
		}
	}
	if (y != height - 1) {
		std::vector<int> currentOrgList = E->getOrgs(x, y+1);
		OrgSense currentOrgSense;
		positionTemp = { 0,1 };
		for (int i = 0; i < currentOrgList.size(); i++) {
			currentOrgSense.ID = currentOrgList[i];
			toReturn.orgsNearby.push_back(std::make_pair(positionTemp, currentOrgSense));
		}
	}
	if (x != 0) {
		std::vector<int> currentOrgList = E->getOrgs(x-1, y);
		OrgSense currentOrgSense;
		positionTemp = { -1,0 };
		for (int i = 0; i < currentOrgList.size(); i++) {
			currentOrgSense.ID = currentOrgList[i];
			toReturn.orgsNearby.push_back(std::make_pair(positionTemp, currentOrgSense));
		}
	}
	if (y != 0) {
		std::vector<int> currentOrgList = E->getOrgs(x, y-1);
		OrgSense currentOrgSense;
		positionTemp = { 0,-1 };
		for (int i = 0; i < currentOrgList.size(); i++) {
			currentOrgSense.ID = currentOrgList[i];
			toReturn.orgsNearby.push_back(std::make_pair(positionTemp, currentOrgSense));
		}
	}

	return toReturn;
}

void Server::addOrg(Organism* org){
	orgList[org->getID()] = org;
	E->addOrg(org->getID(), org->getX(), org->getY());
}


void Server::killOrg(int id){
	E->remOrg(id);
	orgList.erase(orgList.find(id));
}