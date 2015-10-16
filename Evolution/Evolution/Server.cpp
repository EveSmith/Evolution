#include "Server.h"

Server::Server(int WIDTH, int HEIGHT, int INIT_SIZE){
	E = new Environment(WIDTH, HEIGHT);
	for (int i = 0; i < INIT_SIZE; i++){
		OrgList.push_back(new Organism());
	}
}

Server::~Server(){
	delete E;
	for (int i = 0; i < OrgList.size; i++){
		delete OrgList[i];
	}
}