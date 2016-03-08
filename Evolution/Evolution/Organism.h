#ifndef ORGANISM_H
#define ORGANISM_H

#include <string>
#include <array>
#include <queue>
#include <functional>
#include "Messages.h"

std::vector<Intel> dna_to_knowledge(std::string dna);

Traits parse_traits(std::string dna);

class Organism{
public:
	//Generates DNA, spawns organism from the void
	Organism(int w, int h);
	Organism(int w, int h, int initX, int initY, std::string initDNA);

	std::string print();

	int getID();
	int getX();
	int getY();
	Traits getTraits();
	std::string getDNA();

	void receiveUpdate(ServerUpdate update);
	void checkUpdates();
	void setSurroundings(Surroundings sur);
	void injury(int amount);
	void healing(int amount);

	//Send update to the server
	void sendUpdate(std::queue<OrgUpdate>& inbox);
	//Creates the update object, but does not send it
	void updateSelf();

	//Last relevant thoughts
	std::vector<Intel> lastThoughts; //Thoughts
	std::vector<int> lastThoughtRelevancies; //Corresponding relevancies
private:
	int ID;
	Traits traits;
	std::string DNA;
	std::array<int, 2> map_dimensions;

	//Distance around them they can perceive
	int perception = 1;
	//Knowledge of surroundings
	Surroundings surroundings;

	//Compares input observation to current surroundings, decides if situation is similar
	std::vector<std::pair<float, std::string>> compare_surroundings(Situation obs); //Observation
	void complete_action(std::string actionString);

	std::array<int, 2> position;
	ServerUpdate pendingServerUpdate;
	OrgUpdate pendingOrgUpdate;

	//Generates action that the org wants to complete this turn
	//(Puts action in actionPlan)
	void reason();

	void idle();
	void eat();
	void attack(int victimID);
	void move(int deltaX, int deltaY);
	void matingOn();
	void matingOff();

	//Sum of organism's knowledge
	std::vector<Intel> knowledge;
};


#endif //ORGANISM_H