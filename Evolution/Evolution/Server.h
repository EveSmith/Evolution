#ifndef SERVER_H
#define SERVER_H

#include "Environment.h"
#include "Organism.h"
#include <vector>
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

private:
	Environment* E;
	std::vector<Organism*> OrgList;
};



#endif //SERVER_H