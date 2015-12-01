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
	void determineMovement();

	//Send update to the server
	void sendUpdate(std::queue<OrgUpdate> &inbox);
	//Creates the update object, but does not send it
	void updateSelf();

private:
	int ID;
	int health; //org dies when health reaches 0
	std::string DNA;

	//Number of actions organism can plan at once
	int forethought = 3;

	std::array<int, 2> position;
	ServerUpdate pendingServerUpdate;
	OrgUpdate pendingOrgUpdate;

	//Queue of planned actions
	std::queue<std::string> actionPlan;

	//Generates actions that the org wants to complete this turn
	//(Puts actions in actionPlan)
	void reason();

	void move();
	void eat();
	void mate(int mateID);
	void attack(int victimID);
};


#endif //ORGANISM_H