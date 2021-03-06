#ifndef SERVER_H
#define SERVER_H

#include "Environment.h"
#include "Organism.h"
#include <map>
#include <string>


/*
Contains the Environment and knows of all organisms.
Can query Environment for EnviroCell information.
Organisms request actions of server, and server keeps
track and resolves conflicts.
*/
class Server{
public:
	Server(int WIDTH, int HEIGHT, int INIT_SIZE, int GENOME_LENGTH);
	~Server();

	std::string printEnviron(bool grid=true);
	std::string printOrgList();
	std::pair<std::vector<Intel>,std::vector<int>> getThoughtRelevancies();
	State getState();
	Actions getActionsCount();

	bool org_update();
	void server_update();
	Surroundings compileSurroundings(int ID, int x, int y);
	std::string recombineDNA(std::string parent1, std::string parent2);

	void addOrg();
	void addOrg(int initX, int initY, std::string initDNA);
	void killOrg(int x, int y, int id, int size);

private:
	int width, height,genome_length, square_capacity;
	Environment* E;
	std::queue<OrgUpdate> SERVER_INBOX;
	std::map<int, Organism*> ORG_LIST;
	std::map<int, std::array<int, 2>> mateable;
	Actions current_actions_count;
};



#endif //SERVER_H