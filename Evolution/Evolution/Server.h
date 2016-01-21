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
	Server(int WIDTH, int HEIGHT, std::vector<Organism*>* OFFICIAL_ORG_LIST);
	~Server();

	std::string printEnviron(bool grid=true);

	void update(std::queue<OrgUpdate> &inbox);
	void confirmUpdate(OrgUpdate &currUpdate, ServerUpdate &servUpdate);
	std::vector<CellSense> compileSurroundings(int ID, int x, int y, SensoryRequest senseRequest);

	void addOrg(Organism* org);
	void killOrg(int id);

private:
	int width, height;
	Environment* E;
	std::vector<Organism*>* official_org_list;
	std::map<int, Organism*> orgList;
};



#endif //SERVER_H