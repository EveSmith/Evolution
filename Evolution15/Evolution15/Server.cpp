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

		if (currUpdate.eatrequest.request_made) {
			E->changeResources(currUpdate.oldX, currUpdate.oldY, "Food", currUpdate.eatrequest.amount);
			orgList[currUpdate.senderID]->healSelf(currUpdate.eatrequest.amount);
		}
		if (currUpdate.attackrequest.request_made) {
			orgList[currUpdate.attackrequest.victimID]->injureSelf(currUpdate.attackrequest.strength);
		}
		if (currUpdate.materequest.request_made) {
			official_org_list->push_back(new Organism(rand() % width, rand()%height));
			addOrg(official_org_list->back());
		}
		if (currUpdate.healrequest.request_made) {
			orgList[currUpdate.healrequest.friendID]->healSelf(currUpdate.healrequest.strength);
		}

		//confirmUpdate(currUpdate, servUpdate); //Remove and roll into orgs??
		//Move Org from old position to new position
		E->moveOrg(currUpdate.senderID, currUpdate.oldX, currUpdate.oldY, currUpdate.newX, currUpdate.newY);



		//Remove message from inbox
		inbox.pop();


		//Compile server update
		servUpdate.newX = currUpdate.newX;
		servUpdate.newY = currUpdate.newY;
		servUpdate.checked = false;
		servUpdate.surroundings = compileSurroundings(currUpdate.senderID, currUpdate.newX, currUpdate.newY, currUpdate.sensoryrequest);
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
std::vector<CellSense> Server::compileSurroundings(int ID, int x, int y, SensoryRequest senseRequest) {
	std::vector<CellSense> toReturn;
	CellSense currentCell;

	for (int i = 0; i <= senseRequest.perception; i++) {
		if (i == 0) {
			//MIDDLE (CURRENT SPACE ORG IS ON)

			currentCell.deltaX = 0;
			currentCell.deltaY = 0;
			//Get resource info
			currentCell.food = E->getResources(x, y, "Food");
			//Get org info
			std::vector<int> tempOrgList = E->getOrgs(x, y);
			std::vector<OrgSense> orgs;

			//Put org IDs (except own ID) into OrgSense objects
			for (int j = 0; j < tempOrgList.size(); j++) {
				if (tempOrgList[j] != ID) {
					OrgSense tempOrgSense = { tempOrgList[j] };
					orgs.push_back(tempOrgSense);
				}
			}
			currentCell.orgs = orgs;
			toReturn.push_back(currentCell);
		}

		else {
			//UP

			if(y!=0) {
				currentCell.deltaX = 0;
				currentCell.deltaY = -i;
				//Get resource info
				currentCell.food = E->getResources(x, y - i, "Food");
				//Get org info
				std::vector<int> tempOrgList = E->getOrgs(x, y - 1);
				std::vector<OrgSense> orgs;

				//Put org IDs (except own ID) into OrgSense objects
				for (int j = 0; j < tempOrgList.size(); j++) {
					OrgSense tempOrgSense = { tempOrgList[j] };
					orgs.push_back(tempOrgSense);
				}
				currentCell.orgs = orgs;
				toReturn.push_back(currentCell);
			}


			//RIGHT
			if(x!=width-1) {
				currentCell.deltaX = i;
				currentCell.deltaY = 0;
				//Get resource info
				currentCell.food = E->getResources(x + i, y, "Food");
				//Get org info
				std::vector<int> tempOrgList = E->getOrgs(x + 1, y);
				std::vector<OrgSense> orgs;

				//Put org IDs (except own ID) into OrgSense objects
				for (int j = 0; j < tempOrgList.size(); j++) {
					OrgSense tempOrgSense = { tempOrgList[j] };
					orgs.push_back(tempOrgSense);
				}
				currentCell.orgs = orgs;
				toReturn.push_back(currentCell);
			}


			//DOWN

			if(y!=height-1) {
				currentCell.deltaX = 0;
				currentCell.deltaY = i;
				//Get resource info
				currentCell.food = E->getResources(x, y + i, "Food");
				//Get org info
				std::vector<int> tempOrgList = E->getOrgs(x, y + 1);
				std::vector<OrgSense> orgs;

				//Put org IDs (except own ID) into OrgSense objects
				for (int j = 0; j < tempOrgList.size(); j++) {
					OrgSense tempOrgSense = { tempOrgList[j] };
					orgs.push_back(tempOrgSense);
				}
				currentCell.orgs = orgs;
				toReturn.push_back(currentCell);
			}


			//LEFT

			if(x!=0) {
				currentCell.deltaX = -i;
				currentCell.deltaY = 0;
				//Get resource info
				currentCell.food = E->getResources(x - i, y, "Food");
				//Get org info
				std::vector<int> tempOrgList = E->getOrgs(x - i, y);
				std::vector<OrgSense> orgs;

				//Put org IDs (except own ID) into OrgSense objects
				for (int j = 0; j < tempOrgList.size(); j++) {
					OrgSense tempOrgSense = { tempOrgList[j] };
					orgs.push_back(tempOrgSense);
				}
				currentCell.orgs = orgs;
				toReturn.push_back(currentCell);
			}
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