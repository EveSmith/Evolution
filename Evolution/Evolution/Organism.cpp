#include "Organism.h"
#include <iostream>
#include <map>
#include <stdlib.h>
#include <cmath>

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

std::vector<Intel> dna_to_knowledge(std::string dna) {
	std::vector<Intel> knowledge;
	std::string knowledge_dna = dna.substr(6);
	for (int i = 0; i < dna.size() / (INTEL_LENGTH*GENE_LENGTH); i++) {
		Situation currentSit;
		Intel currentIntel;
		std::string intel_slice = dna.substr(i*INTEL_LENGTH, INTEL_LENGTH*GENE_LENGTH);
		std::cout << "Intel Slice: " << intel_slice << std::endl;
		//Subject
		std::cout << "Parsing Subject... ";
		std::string subjects[] = {"Org", "Org", "Org", "Org", "Org", "Food", "Food", "Food", "Food", "Food"};
		int subjectVal = bin_to_int(intel_slice.substr(0, GENE_LENGTH));
		currentSit.subject = subjects[subjectVal];
		std::cout << subjectVal << std::endl;
		//Trait
		std::cout << "Parsing Trait... ";
		int traitVal = bin_to_int(intel_slice.substr(GENE_LENGTH, GENE_LENGTH));
		std::string traits[] = { "Mateable", "Proximity", "Proximity", "Color", "Color", "Size", "Size", "Newborn" };
		currentSit.trait = traits[traitVal];
		std::cout << traitVal << std::endl;
		//Value
		std::cout << "Parsing Value... ";
		int valueVal = bin_to_int(intel_slice.substr(2 * GENE_LENGTH, GENE_LENGTH));
		if (currentSit.trait == "Mateable") {
			std::string values[] = { "yes", "yes", "yes", "yes", "no", "no", "no", "no" };
			currentSit.value = values[valueVal];
		}
		else if (currentSit.trait == "Proximity") {
			std::string values[] = { "near", "near", "near", "near", "far", "far", "far", "far" }; //EVENUTALLY INCLUDE MID??
			currentSit.value = values[valueVal];
		}
		else if (currentSit.trait == "Color") {
			currentSit.value = valueVal;
		}
		else if (currentSit.trait == "Size") {
			std::string values[] = { "smaller", "smaller", "smaller", "similar", "similar", "larger", "larger", "larger" };
			currentSit.value = values[valueVal];
		}
		else {
			std::string values[] = { "yes", "yes", "yes", "yes", "no", "no", "no", "no" };
			currentSit.value = values[valueVal];
		}
		std::cout << valueVal << std::endl;
		//Action
		std::cout << "Parsing Action... ";
		currentIntel.situation = currentSit;
		int actionVal = bin_to_int(intel_slice.substr(3 * GENE_LENGTH, GENE_LENGTH));
		std::string actions[] = { "Ignore", "Eat", "Eat", "Attack", "Move Away", "Move Toward", "Mating On", "Mating Off" };
		currentIntel.action = actions[actionVal];
		std::cout << actionVal << std::endl;
		//Rating
		std::cout << "Parsing Rating... " << std::endl;
		currentIntel.rating = bin_to_int(intel_slice.substr(4 * GENE_LENGTH, GENE_LENGTH));

		knowledge.push_back(currentIntel);
	}
	return knowledge;
}


Traits parse_traits(std::string dna) {
	Traits traits;
	traits.Mateable = false;
	traits.Newborn = true;
	traits.Health = 100;
	traits.Color = bin_to_int(dna.substr(0, 3));
	traits.Size = bin_to_int(dna.substr(3, 3));
	return traits;
}


/*
Organism handles:
	Health -- Server does not need to know health of organisms
*/

Organism::Organism(){
	//Unique ID for each organism, hopefully.
	this->ID = UNIVERSAL_ID;
	UNIVERSAL_ID++;

	//Generate some binary DNA
	DNA = "";
	for (int i = 0; i < (GENE_LENGTH*INTEL_LENGTH*1)+6; i++){
		DNA.append(std::to_string(rand() % 2));
	}

	//Set traits
	this->traits = parse_traits(DNA);

	//set org position to 0,0
	position.fill(0);

	pendingServerUpdate = { true, 0, 0 };

	//establish default surroundings?
}

Organism::Organism(int initX, int initY, std::string initDNA){
	//Unique ID for each organism, hopefully.
	this->ID = UNIVERSAL_ID;
	UNIVERSAL_ID++;

	DNA = initDNA;

	//Set traits
	this->traits = parse_traits(DNA);

	//set org position to 0,0
	position[0] = initX;
	position[1] = initY;

	pendingServerUpdate = { true, 0, 0 };

	//establish default surroundings?
}

std::string Organism::print(){
	std::string info = "Organism: " + std::to_string(ID) + "\n\tPosition: " +
		std::to_string(position[0]) + "," + std::to_string(position[1]) + "\n" +
		"\tHealth: " + std::to_string(traits.Health) + "\n" +
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
	traits.Health -= amount;
	if (traits.Health < 0) { traits.Health = 0; }
}


//Send update to server
void Organism::sendUpdate(std::queue<OrgUpdate> &inbox){
	inbox.push(pendingOrgUpdate);
	pendingOrgUpdate.eatrequest.request_made = false;
	pendingOrgUpdate.attackrequest.request_made = false;
	pendingOrgUpdate.materequest.request_made = false;
}


void Organism::updateSelf(){
	pendingOrgUpdate.senderID = ID;
	traits.Health -= 4;

	if (traits.Health <= 0) {
		pendingOrgUpdate.alive = false;
	}

	pendingOrgUpdate.oldX = position[0];
	pendingOrgUpdate.oldY = position[1];

	if (!traits.Newborn) {
		reason();

		while (!actionPlan.empty()) {
			actionPlan.front()();
			actionPlan.pop();
		}
	}
	else {
		move(0, 0);
		traits.Newborn = false;
	}
	pendingOrgUpdate.sensoryrequest = { perception };

	std::cout << std::endl;
}

//Returns a map of locations to their associated comparison ratings
std::map<std::array<int,2>, float> Organism::compare_surroundings(Situation sit) {
	float comparison_rating = 0;
	std::array<int, 2> location_of_interest;
	std::map<std::array<int, 2>, float> toReturn;

	if (sit.subject == "Org" && !surroundings.orgsNearby.empty()) {
		for (int i = 0; i < surroundings.orgsNearby.size(); i++) {
			location_of_interest = surroundings.orgsNearby[i].first;
			OrgSense current_org = surroundings.orgsNearby[i].second;
			if (sit.trait == "Mateable") {
				//If mating requirements match
				if ((current_org.mateable && sit.value == "yes") || (!current_org.mateable && sit.value == "no")) {
					comparison_rating += 1;
				}
			}
			else if (sit.trait == "Proximity") {
				float distance = sqrt(pow(location_of_interest[0],2) + pow(location_of_interest[1],2));
				if (distance<1) {
					if(sit.value == "near"){ comparison_rating += 1; }
					else if (sit.value == "far") { comparison_rating += 0.5; }
				}
				else if (distance>=1) {
					if (sit.value == "far") { comparison_rating += 1; }
					else if (sit.value == "near") { comparison_rating += 0.5; }
				}
				//ADD MIDDLE RANGE IF I DECIDE TO DO THAT
			}
			else if (sit.trait == "Color") {
				if (current_org.color == std::stoi(sit.value)) {
					comparison_rating += 1;
				}
				else if (current_org.color == (std::stoi(sit.value) - 1) % 8 || current_org.color == (std::stoi(sit.value) + 1) % 8) {
					comparison_rating += 0.5;
				}
			}
			else if (sit.trait == "Size") {
				if (current_org.size < traits.Size - 1 && sit.value == "smaller") {
					comparison_rating += 1;
				}
				else if (current_org.size >= traits.Size - 1 && current_org.size <= traits.Size+1 && sit.value == "similar") {
					comparison_rating += 1;
				}
				if (current_org.size > traits.Size + 1 && sit.value == "larger") {
					comparison_rating += 1;
				}
			}
			else if (sit.trait == "Newborn") {
				if ((current_org.newborn && sit.value == "yes") || (!current_org.newborn && sit.value == "no")) {
					comparison_rating += 1;
				}
			}
			toReturn[location_of_interest] += comparison_rating;
		}
	}
	else if (sit.subject == "Food") {
		for (int i = 0; i < surroundings.foodNearby.size(); i++) {
			if (sit.trait == "Proximity") {
				float distance = sqrt(pow(location_of_interest[0], 2) + pow(location_of_interest[1], 2));
				if (distance<1) {
					if (sit.value == "near") { comparison_rating += 1; }
					else if (sit.value == "far") { comparison_rating += 0.5; }
				}
				else if (distance >= 1) {
					if (sit.value == "far") { comparison_rating += 1; }
					else if (sit.value == "near") { comparison_rating += 0.5; }
				}
				//ADD MIDDLE RANGE IF I DECIDE TO DO THAT
			}
			else if (sit.trait == "Size") {
				//Measures amount of food relative to how much health the org has
				int foodValue = surroundings.foodNearby[i].second;
				if (foodValue < traits.Health - 20 && sit.value == "smaller") {
					comparison_rating += 1;
				}
				else if (foodValue >= traits.Health - 20 && foodValue <= traits.Health + 20 && sit.value == "similar") {
					comparison_rating += 1;
				}
				if (foodValue > traits.Health + 20 && sit.value == "larger") {
					comparison_rating += 1;
				}
			}
			toReturn[location_of_interest] += comparison_rating;
		}
	}
	return toReturn;
}

void Organism::reason(){
	std::map<std::string, std::pair<float, std::array<int,2>>> action_ratings; //map<action, pair<rating, location of interest>>
	for (int i = 0; i < knowledge.size(); i++) {
		//Add new action if not seen before
		if (action_ratings.find(knowledge[i].action) == action_ratings.end()) {
			action_ratings[knowledge[i].action];
		}
		std::pair<float, std::array<int, 2>> current_action = compare_surroundings(knowledge[i].situation);//FIX NOW
		current_action.first = current_action.first * knowledge[i].rating;
		action_ratings[knowledge[i].action].first += current_action.first;
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
