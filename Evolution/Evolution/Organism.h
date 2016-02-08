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
	Organism();
	Organism(int initX, int initY, std::string initDNA);

	std::string print();

	int getID();
	int getX();
	int getY();
	std::string getDNA();

	void receiveUpdate(ServerUpdate update);
	void checkUpdates();
	void injureSelf(int amount);

	//Send update to the server
	void sendUpdate(std::queue<OrgUpdate> &inbox);
	//Creates the update object, but does not send it
	void updateSelf();

private:
	int ID;
	Traits traits;
	std::string DNA;

	//Distance around them they can perceive
	int perception = 1;
	//Knowledge of surroundings
	Surroundings surroundings;

	//Compares input observation to current surroundings, decides if situation is similar
	std::vector<std::pair<float, std::string>> compare_surroundings(Situation obs); //Observation
	std::pair<std::function<void(Params p)>,Params> generate_action_function(std::string actionString);

	std::array<int, 2> position;
	ServerUpdate pendingServerUpdate;
	OrgUpdate pendingOrgUpdate;

	//Queue of planned actions
	std::pair<std::function<void(Params p)>, Params> actionPlan;

	//Generates action that the org wants to complete this turn
	//(Puts action in actionPlan)
	void reason();

	void idle(Params p);
	void eat(Params p);
	void attack(Params p);
	void move(Params p);
	void toggleMating(Params p);

	//Sum of organism's knowledge
	std::vector<Intel> knowledge;
};


#endif //ORGANISM_H