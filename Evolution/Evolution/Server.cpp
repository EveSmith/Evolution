#include "Server.h"
#include <algorithm>


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
	while (!inbox.empty()){
		currUpdate = inbox.front();
		if (currUpdate.health <= 0){
			killOrg(currUpdate.senderID);
		}

		//confirmUpdate(currUpdate, servUpdate);
		E->moveOrg(currUpdate.senderID, currUpdate.oldX, currUpdate.oldY, currUpdate.newX, currUpdate.newY);
		inbox.pop();
		servUpdate.newX = currUpdate.newX;
		servUpdate.newY = currUpdate.newY;
		servUpdate.cellValue = E->getValue(servUpdate.newX, servUpdate.newY);
		//CANNOT STAY LIKE THIS. DEATH/BIRTH WILL MAKE ID!=INDEX!!!
		std::vector<Organism*>::iterator it = std::find_if(orgList.begin(), orgList.end(), [currUpdate](Organism* org){return org->getID() == currUpdate.senderID; })
		(*it)->receiveUpdate(servUpdate);
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
	orgList.push_back(org);
	E->addOrg(org->getID(), org->getX(), org->getY());
}


void Server::killOrg(int id){
	E->remOrg(id);
	Organism* copy = *std::find_if(orgList.begin(), orgList.end(), [id](Organism* org){return org->getID() == id; });
	orgList.erase(std::find_if(orgList.begin(), orgList.end(), [id](Organism* org){return org->getID() == id; }));
	delete copy;
}