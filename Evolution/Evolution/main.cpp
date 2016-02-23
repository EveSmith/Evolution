#include <random>
#include <string>
#include <vector>
#include <iostream>
#include <time.h>
#include "Server.h"

std::queue<OrgUpdate> SERVER_INBOX;
std::vector<Organism*> ORG_LIST;

//TO DO: organisms spawn as multithreaded objects
void spawnNewOrg(Server* server, int w, int h){
	Organism* newOrgPointer = new Organism();
	ORG_LIST.push_back(newOrgPointer);

	server->addOrg(newOrgPointer);
	newOrgPointer->setSurroundings(server->compileSurroundings(newOrgPointer->getID(), newOrgPointer->getX(), newOrgPointer->getY()));
}

std::string printOrgList(){
	std::string list = "Organism List:\n";
	for (int i = 0; i < ORG_LIST.size(); i++){
		list.append(ORG_LIST[i]->print() + "\n");
	}
	return list;
}


int main(){
	int SEED = time(NULL);
	srand(SEED);

	//Organism neworg = Organism();
	////Print Org Knowledge
	//std::cout << "Organism DNA: " + neworg.getDNA() << std::endl;
	//std::vector<Intel> knowledge = dna_to_knowledge(neworg.getDNA());
	//for (int i = 0; i < knowledge.size(); i++) {
	//	std::cout << "Subject: " << knowledge[i].situation.subject << std::endl;
	//	std::cout << "Trait: " << knowledge[i].situation.trait << std::endl;
	//	std::cout << "Value: " << knowledge[i].situation.value << std::endl;
	//	std::cout << "Action: " << knowledge[i].action << std::endl;
	//	std::cout << "Rating: " << knowledge[i].rating << std::endl;
	//}
	//std::cout << std::endl;

	//neworg.updateSelf();



	//std::string input;
	//std::cin >> input;

	int WIDTH = 5;
	int HEIGHT = 5;
	int INIT_SIZE = 1;

	Server* server = new Server(WIDTH, HEIGHT, &ORG_LIST);

	/*for (int i = 0; i < INIT_SIZE; i++){
		spawnNewOrg(server, WIDTH, HEIGHT);
	}*/

	//If food far, move toward it,
	//if food near, eat it,
	Organism* newOrgPointer = new Organism();//3,3,"000000111001111101100111001001001111");
	ORG_LIST.push_back(newOrgPointer);

	server->addOrg(newOrgPointer);
	newOrgPointer->setSurroundings(server->compileSurroundings(newOrgPointer->getID(), newOrgPointer->getX(), newOrgPointer->getY()));
	

	std::string input;
	while (input != "end"){
		//TO DO: CHANGE TO "SIMULTANEOUS" BEHAVIOR
		for (int i = 0; i < ORG_LIST.size(); i++){
			ORG_LIST[i]->updateSelf();
			ORG_LIST[i]->sendUpdate(SERVER_INBOX);
		}
		server->update(SERVER_INBOX);

		std::cout << printOrgList();
		std::cout << server->printEnviron();

		std::cout << "Enter 'end' to end. Enter anything else to continue...\n";
		std::cin >> input;
	}

	return 0;
}