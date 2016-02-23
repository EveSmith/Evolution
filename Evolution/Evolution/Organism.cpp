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
	if (binary[2] == '1') {
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
		std::string intel_slice = knowledge_dna.substr(i*INTEL_LENGTH, INTEL_LENGTH*GENE_LENGTH);
		//std::cout << "Intel Slice: " << intel_slice << std::endl;
		//Subject
		//std::cout << "Parsing Subject... ";
		std::string subjects[] = {"Org", "Org", "Org", "Org", "Org", "Food", "Food", "Food", "Food", "Food"};
		int subjectVal = bin_to_int(intel_slice.substr(0, GENE_LENGTH));
		currentSit.subject = subjects[subjectVal];
		//Trait
		//std::cout << "Parsing Trait... ";
		int traitVal = bin_to_int(intel_slice.substr(GENE_LENGTH, GENE_LENGTH));
		std::string traits[] = { "Mateable", "Proximity", "Proximity", "Color", "Color", "Size", "Size", "Newborn" };
		currentSit.trait = traits[traitVal];
		//Value
		//std::cout << "Parsing Value... ";
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
		//std::cout << valueVal << std::endl;
		//Action
		//std::cout << "Parsing Action... ";
		currentIntel.situation = currentSit;
		int actionVal = bin_to_int(intel_slice.substr(3 * GENE_LENGTH, GENE_LENGTH));
		std::string actions[] = { "Idle", "Eat", "Eat", "Attack", "MoveAway", "MoveToward", "MatingOn", "MatingOff" };
		currentIntel.action = actions[actionVal];
		//std::cout << actionVal << std::endl;
		//Rating
		//std::cout << "Parsing Rating... " << std::endl;
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
	for (int i = 0; i < (GENE_LENGTH*INTEL_LENGTH*50)+6; i++){
		DNA.append(std::to_string(rand() % 2));
	}
	this->knowledge = dna_to_knowledge(DNA);

	//Set traits
	this->traits = parse_traits(DNA);

	//set org position to 0,0
	position.fill(0);

	pendingServerUpdate = { true, 0, 0 };
}

Organism::Organism(int initX, int initY, std::string initDNA){
	//Unique ID for each organism, hopefully.
	this->ID = UNIVERSAL_ID;
	UNIVERSAL_ID++;

	DNA = initDNA;
	this->knowledge = dna_to_knowledge(DNA);

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
		"\tHealth: " + std::to_string(traits.Health) + "\n";
		//"\tDNA: " + DNA + "\n";
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

void Organism::setSurroundings(Surroundings sur) {
	surroundings = sur;
}

void Organism::injury(int amount){
	traits.Health -= amount;
	if (traits.Health < 0) { traits.Health = 0; }
}

void Organism::healing(int amount) {
	traits.Health += amount;
	if (traits.Health > 100) { traits.Health = 100;	}
}

//Send update to server
void Organism::sendUpdate(std::queue<OrgUpdate> &inbox){
	inbox.push(pendingOrgUpdate);
	pendingOrgUpdate.action = "Idle";
}


void Organism::updateSelf(){
	pendingOrgUpdate.senderID = ID;
	traits.Health -= 4;

	if (traits.Health <= 0) {
		pendingOrgUpdate.alive = false;
	}

	pendingOrgUpdate.oldX = position[0];
	pendingOrgUpdate.oldY = position[1];

	//ADD NEWBORN STUFF
	reason();

}

//Returns comparison rating, as well as an optional string to append to the action (specify locations, org IDs, etc.)
std::vector<std::pair<float, std::string>> Organism::compare_surroundings(Situation sit) {
	float comparison_rating = 0;
	std::array<int, 2> location_of_interest = {position[0], position[1]};
	std::vector<std::pair<float, std::string>> toReturn;
	std::cout << "\tOrganism noticed ";
	if (sit.subject == "Org" && !surroundings.orgsNearby.empty()) {
		std::cout << "another organism ";
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
					if (sit.value == "near") { std::cout << "nearby." << std::endl; comparison_rating += 1; }
					else if (sit.value == "far") { std::cout << "far away." << std::endl; comparison_rating += 0.5; }
				}
				else if (distance>=1) {
					if (sit.value == "far") { std::cout << "far away." << std::endl; comparison_rating += 1; }
					else if (sit.value == "near") { std::cout << "nearby." << std::endl; comparison_rating += 0.5; }
				}
				//ADD MIDDLE RANGE IF I DECIDE TO DO THAT
			}
			else if (sit.trait == "Color") {
				std::cout << "of color " << current_org.color << "." << std::endl;
				if (current_org.color == std::stoi(sit.value)) {
					comparison_rating += 1;
				}
				else if (current_org.color == (std::stoi(sit.value) - 1) % 8 || current_org.color == (std::stoi(sit.value) + 1) % 8) {
					comparison_rating += 0.5;
				}
			}
			else if (sit.trait == "Size") {
				std::cout << "of size " << current_org.size << "." << std::endl;
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
				std::cout << "who is newborn." << std::endl;
				if ((current_org.newborn && sit.value == "yes") || (!current_org.newborn && sit.value == "no")) {
					comparison_rating += 1;
				}
			}
			toReturn.push_back(std::make_pair(comparison_rating, std::to_string(location_of_interest[0]) + "," + std::to_string(location_of_interest[1])));
		}
	}
	else if (sit.subject == "Food") {
		std::cout << "some food ";
		for (int i = 0; i < surroundings.foodNearby.size(); i++) {
			location_of_interest = surroundings.foodNearby[i].first;
			int current_amt = surroundings.foodNearby[i].second;
			if (sit.trait == "Proximity") {
				float distance = sqrt(pow(location_of_interest[0], 2) + pow(location_of_interest[1], 2));
				if (distance<1) {
					if (sit.value == "near") { std::cout << "nearby." << std::endl; comparison_rating += 1; }
					else if (sit.value == "far") { std::cout << "far away." << std::endl; comparison_rating += 0.5; }
				}
				else if (distance >= 1) {
					if (sit.value == "far") { std::cout << "far away." << std::endl; comparison_rating += 1; }
					else if (sit.value == "near") { std::cout << "nearby." << std::endl; comparison_rating += 0.5; }
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
			toReturn.push_back(std::make_pair(comparison_rating, std::to_string(location_of_interest[0])+","+std::to_string(location_of_interest[1])));
		}
	}
	return toReturn;
}

void Organism::complete_action(std::string actionString) {
	std::size_t split = actionString.find(" "); //Find delim, if exists
	std::string actionSuffix = actionString.substr(split); //Split on delim
	std::string action = actionString.substr(0, actionString.size() - actionSuffix.size()); //Get action name
	int deltaY, deltaX;
	int targetID;
	if (!actionSuffix.empty()) {
		actionSuffix = actionSuffix.substr(1); //strip whitespace at beginning
		split = actionSuffix.find(",");
		if (split != actionSuffix.size()) { //If suffix = coordinates
			std::string temp1 = actionSuffix.substr(split); //Split coords
			temp1 = temp1.substr(1); //Strip comma
			deltaY = stoi(temp1);
			deltaX = stoi(actionSuffix.substr(0, actionSuffix.size() - temp1.size()));
		}
		else {
			targetID = stoi(actionSuffix);
		}
	}
	if (action == "Idle") {
		idle();
	}
	else if (action == "Eat") {
		eat();
	}
	else if (action == "Attack") {
		attack(targetID);
	}
	else if (action == "MoveToward") {
		move(deltaX, deltaY);
	}
	else if (action == "MoveAway") {
		move(-deltaX, -deltaY);
	}
	else if (action == "MatingOn") {
		toggleMating();
	}
	else if (action == "MatingOff") {
		toggleMating();
	}

}

void Organism::reason(){
	std::map<std::string, float> action_ratings;
	//Compare surroundings for every piece of knowledge, append action suffixes, factor in rating
	for (int i = 0; i < knowledge.size(); i++) {
		//Assess the relevance of each thought
		std::vector<std::pair<float, std::string>> current_location_action_ratings = compare_surroundings(knowledge[i].situation);
		//For every "thought-relevance"-thought pair...
		for (int j = 0; j < current_location_action_ratings.size(); j++) {
			//Add the action rating to the relevance
			current_location_action_ratings[j].first += knowledge[i].rating;
			//Add action suffix information to action
			std::string full_action = knowledge[i].action + " " + current_location_action_ratings[j].second;
			if (action_ratings.find(full_action) == action_ratings.end()) {
				action_ratings[full_action] = 0;
			}
			action_ratings[full_action] += current_location_action_ratings[j].first;
		}
	}
	//Find action with highest rating
	std::pair<std::string, float> max_action = {"Idle", 0.0};
	bool beginning = true;
	
	for (auto it = action_ratings.begin(); it != action_ratings.end(); it++) {
		if (beginning) {
			max_action = std::make_pair(it->first, it->second);
			beginning = false;
		}
		else {
			if (it->second > max_action.second) {
				max_action = std::make_pair(it->first, it->second);
			}
			else if (it->second == max_action.second) {
				if (rand()%2 == 0) {
					max_action = std::make_pair(it->first, it->second);
				}
			}
		}
	}
	complete_action(max_action.first);
}

void Organism::idle() {
	std::cout << "Organism is idling" << std::endl;
	pendingOrgUpdate.action = "Idle";
}

void Organism::eat(){
	std::cout << "Organism is eating ("<< 5 << ")" << std::endl;
	pendingOrgUpdate.action = "Eat";
	pendingOrgUpdate.amount = 5;
}

void Organism::attack(int targetID){
	std::cout << "Organism is attacking organism "<<targetID<<" ("<< 5 << ")" << std::endl;
	pendingOrgUpdate.action = "Attack";
	pendingOrgUpdate.targetID = targetID;
	pendingOrgUpdate.amount = traits.Size*2;
}

void Organism::move(int deltaX, int deltaY) {
	std::cout << "Organism is moved: deltaX="<<deltaX<<" and deltaY="<<deltaY<< std::endl;
	pendingOrgUpdate.action = "Move";
	pendingOrgUpdate.newX = this->position[0] + deltaX;
	pendingOrgUpdate.newY = this->position[1] + deltaY;
}

void Organism::toggleMating() {
	std::cout << "Organism toggled mating from "<<traits.Mateable<< " to "<< !traits.Mateable<< std::endl;
	pendingOrgUpdate.action = "ToggleMating";
	traits.Mateable = !traits.Mateable;
}