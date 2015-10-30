#include <random>
#include <string>
#include <vector>
#include <iostream>
#include "Server.h"

std::queue<OrgUpdate> SERVER_INBOX;
std::vector<Organism*> ORG_LIST;

//TO DO: organisms spawn as multithreaded objects
void spawnNewOrg(Server* server, int w, int h){
	ORG_LIST.push_back(new Organism(rand()%w, rand()%h));
	server->addOrg(ORG_LIST.back());
}

std::string printOrgList(){
	std::string list = "Organism List:\n";
	for (int i = 0; i < ORG_LIST.size(); i++){
		list.append(ORG_LIST[i]->print() + "\n");
	}
	return list;
}


int main(){
	int SEED = 0;
	srand(SEED);

	int WIDTH = 5;
	int HEIGHT = 5;
	int INIT_SIZE = 2;

	Server* server = new Server(WIDTH, HEIGHT);

	for (int i = 0; i < INIT_SIZE; i++){
		spawnNewOrg(server, WIDTH, HEIGHT);
	}

	std::string input;

	while (input != "end"){
		//TO DO: CHANGE TO MULTITHREADED "SIMULTANEOUS" BEHAVIOR
		for (int i = 0; i < ORG_LIST.size(); i++){
			ORG_LIST[i]->update(SERVER_INBOX);
		}
		std::cout << printOrgList();
		server->update(SERVER_INBOX);

		std::cout << server->printEnviron();
		std::cout << printOrgList();

		std::cout << "Enter 'end' to end. Enter anything else to continue...\n";
		std::cin >> input;
	}

	return 0;
}