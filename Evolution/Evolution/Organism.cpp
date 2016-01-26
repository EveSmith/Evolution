#include "Organism.h"
#include <iostream>
#include <map>
#include <stdlib.h>

static int UNIVERSAL_ID = 0;
static int GENE_LENGTH = 3;
static int INTEL_LENGTH = 5;

int bin_to_int(std::string binary) {
	int answer = 0;
	if (binary[0] == '1') {
		answer += 4;
	}
	if (binary[1] == '1') {
		answer += 2;
	}
	if (binary[0] == '1') {
		answer += 1;
	}
	return answer;
}

std::vector<Intel> parse_dna(std::string dna) {
	std::vector<Intel> knowledge;
	for (int i = 0; i < dna.size() / INTEL_LENGTH; i++) {
		Observation currentObs;
		Intel currentIntel;
		std::string intel_slice = dna.substr(i*INTEL_LENGTH, INTEL_LENGTH*GENE_LENGTH);
		std::cout << intel_slice;
		//Subject
		std::string subjects[] = {"Org", "Org", "Org", "Org", "Org", "Food", "Food", "Food", "Food", "Food"};
		int subjectVal = bin_to_int(intel_slice.substr(0, GENE_LENGTH));
		currentObs.subject = subjects[subjectVal];
		//Trait
		int traitVal = bin_to_int(intel_slice.substr(GENE_LENGTH, GENE_LENGTH));
		std::string traits[] = { "Mateable", "Proximity", "Proximity", "Color", "Color", "Size", "Size", "Newborn" };
		currentObs.trait = traits[traitVal];
		//Value
		int valueVal = bin_to_int(intel_slice.substr(2 * GENE_LENGTH, GENE_LENGTH));
		if (currentObs.trait == "Mateable") {
			std::string values[] = { "yes", "yes", "yes", "yes", "no", "no", "no", "no" };
			currentObs.value = values[valueVal];
		}
		else if (currentObs.trait == "Proximity") {
			std::string values[] = { "near", "near", "near", "mid", "mid", "far", "far", "far" };
			currentObs.value = values[valueVal];
		}
		else if (currentObs.trait == "Color") {
			currentObs.value = valueVal;
		}
		else if (currentObs.trait == "Size") {
			std::string values[] = { "smaller", "smaller", "smaller", "similar", "similar", "larger", "larger", "larger" };
			currentObs.value = values[valueVal];
		}
		else {
			std::string values[] = { "yes", "yes", "yes", "yes", "no", "no", "no", "no" };
			currentObs.value = values[valueVal];
		}
		//Action
		currentIntel.observation = currentObs;
		int actionVal = bin_to_int(intel_slice.substr(3 * GENE_LENGTH, GENE_LENGTH));
		std::string actions[] = { "Ignore", "Eat", "Eat", "Attack", "Move Away", "Move Toward", "Mating On", "Mating Off" };
		currentIntel.action = actions[actionVal];
		//Rating
		currentIntel.rating = bin_to_int(intel_slice.substr(4 * GENE_LENGTH, GENE_LENGTH));

		knowledge.push_back(currentIntel);
	}
	return knowledge;
}

/*
Organism handles:
	Health -- Server does not need to know health of organisms
*/

Organism::Organism(){
	//Unique ID for each organism, hopefully.
	this->ID = UNIVERSAL_ID;
	UNIVERSAL_ID++;

	this->health = 100;

	//Generate some binary DNA
	DNA = "";
	for (int i = 0; i < GENE_LENGTH*INTEL_LENGTH*1; i++){
		DNA.append(std::to_string(rand() % 2));
	}
	std::cout << DNA;

	//set org position to 0,0
	position.fill(0);

	pendingServerUpdate = { true, 0, 0 };
	newborn = true;

	//establish default surroundings?
}

Organism::Organism(int initX, int initY, std::string initDNA){
	//Unique ID for each organism, hopefully.
	this->ID = UNIVERSAL_ID;
	UNIVERSAL_ID++;

	this->health = 100;

	//Generate some length of DNA (currently length 1)
	DNA = initDNA;

	//set org position to 0,0
	position[0] = initX;
	position[1] = initY;

	pendingServerUpdate = { true, 0, 0 };
	newborn = true;

	//establish default surroundings?
}

std::string Organism::print(){
	std::string info = "Organism: " + std::to_string(ID) + "\n\tPosition: " +
		std::to_string(position[0]) + "," + std::to_string(position[1]) + "\n" +
		"\tHealth: " + std::to_string(health) + "\n" +
		"\tDNA: " + DNA + "\n";
		//"\tResources on position: " + std::to_string(surroundings[0].food) + "\n";
	return info;
}


int Organism::getID(){
	return ID;
}

int Organism::getX(){
	return position[0];
}

int Organism::getY(){
	return position[1];
}

std::string Organism::getDNA() {
	return DNA;
}

void Organism::receiveUpdate(ServerUpdate update){
	this->pendingServerUpdate = update;
	checkUpdates();
}

void Organism::checkUpdates(){
	if (!pendingServerUpdate.checked){
		position[0] = pendingServerUpdate.newX;
		position[1] = pendingServerUpdate.newY;
		surroundings = pendingServerUpdate.surroundings;
	}
	pendingServerUpdate.checked = true;
}

void Organism::injureSelf(int amount){
	health -= amount;
	if (health < 0) { health = 0; }
}

void Organism::healSelf(int amount){
	health += amount;
	if (health > 100) { health = 100; }
}


//Send update to server
void Organism::sendUpdate(std::queue<OrgUpdate> &inbox){
	inbox.push(pendingOrgUpdate);
	pendingOrgUpdate.eatrequest.request_made = false;
	pendingOrgUpdate.attackrequest.request_made = false;
	pendingOrgUpdate.materequest.request_made = false;
	pendingOrgUpdate.healrequest.request_made = false;
}


void Organism::updateSelf(){
	pendingOrgUpdate.senderID = ID;
	health -= 4;

	if (health <= 0) {
		pendingOrgUpdate.alive = false;
	}

	pendingOrgUpdate.oldX = position[0];
	pendingOrgUpdate.oldY = position[1];

	if (!newborn) {
		reason();

		while (!actionPlan.empty()) {
			actionPlan.front()();
			actionPlan.pop();
		}
	}
	else {
		move(0, 0);
		newborn = false;
	}
	pendingOrgUpdate.sensoryrequest = { perception };

	std::cout << std::endl;
}


float Organism::compare_surroundings(Observation obs) {
	float comparison_rating = 0;
	if (obs.subject == "Org" && !surroundings.orgsNearby.empty()) {
		comparison_rating++;
	}
	else if (obs.subject == "Food") {

	}
	return comparison_rating;
}


void Organism::reason(){
	std::map<std::string, float> action_ratings;
	for (int i = 0; i < knowledge.size(); i++) {
		//Add new action if not seen before
		if (action_ratings.find(knowledge[i].action) == action_ratings.end()) {
			action_ratings[knowledge[i].action] = 0;
		}
		float current_action_weight = compare_surroundings(knowledge[i].observation) * knowledge[i].rating;
		action_ratings[knowledge[i].action] += current_action_weight;
	}
}

void Organism::move(int deltaX, int deltaY){
	pendingOrgUpdate.newX = this->position[0]+deltaX;
	pendingOrgUpdate.newY = this->position[1]+deltaY;
}

void Organism::eat(){
	pendingOrgUpdate.eatrequest.request_made = true;
	pendingOrgUpdate.eatrequest.amount = 5;
}

void Organism::mate(int mateID){
	pendingOrgUpdate.materequest.request_made = true;
	pendingOrgUpdate.materequest.mateID = mateID;
}

void Organism::attack(int victimID){
	pendingOrgUpdate.attackrequest.request_made = true;
	pendingOrgUpdate.attackrequest.victimID = victimID;
	pendingOrgUpdate.attackrequest.strength = 10;
}
