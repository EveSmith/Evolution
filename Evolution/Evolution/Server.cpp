#include "Server.h"
#include <algorithm>
#include <iostream>


Server::Server(int WIDTH, int HEIGHT){
	E = new Environment(WIDTH, HEIGHT);
	width = WIDTH;
	height = HEIGHT;
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
		//If Org has 0 health, kill it
		if (!currUpdate.alive){
			killOrg(currUpdate.senderID);
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


void Server::addOrg(Organism* org){
	orgList[org->getID()] = org;
	E->addOrg(org->getID(), org->getX(), org->getY());
}


void Server::killOrg(int id){
	E->remOrg(id);
	orgList.erase(orgList.find(id));
}