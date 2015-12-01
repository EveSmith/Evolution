#ifndef ORGANISM_H
#define ORGANISM_H

#include <string>
#include <array>
#include <queue>
#include "Messages.h"

class Organism{
public:
	//Generates DNA, spawns organism from the void
	Organism();
	Organism(int initX, int initY);
	Organism(std::string str);

	std::string print();

	int getID();
	int getX();
	int getY();

	void receiveUpdate(ServerUpdate update);
	void checkUpdates();
	void adjustHealth(int delta);
	void move();
	void sendUpdate(std::queue<OrgUpdate> &inbox);

private:
	int ID;
	int health; //org dies when health reaches 0
	std::string DNA;

	std::array<int, 2> position;
	ServerUpdate pendingUpdate;
};


#endif //ORGANISM_H