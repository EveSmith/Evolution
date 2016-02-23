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
	Server(int WIDTH, int HEIGHT, int INIT_SIZE);
	~Server();

	std::string printEnviron(bool grid=true);
	std::string printOrgList();

	void org_update();
	void server_update();
	void confirmUpdate(OrgUpdate &currUpdate, ServerUpdate &servUpdate);
	Surroundings compileSurroundings(int ID, int x, int y);
	Surroundings birthUpdate(int ID, int x, int y);

	void addOrg();
	void addOrg(int initX, int initY, std::string initDNA);
	void killOrg(int id);
	

private:
	int width, height;
	Environment* E;
	std::queue<OrgUpdate> SERVER_INBOX;
	std::map<int, Organism*> ORG_LIST;
	std::map<int, std::array<int, 2>> mateable;
};



#endif //SERVER_H