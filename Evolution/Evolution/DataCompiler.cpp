#include <fstream>
#include "DataCompiler.h"


Data_Compiler::Data_Compiler(Server* s) {
	server = s;
}

void Data_Compiler::updateInfo() {
	std::pair<std::vector<Intel>, std::vector<int>> thoughtInfo = server->getThoughtRelevancies();
	THOUGHTS = thoughtInfo.first;
	THOUGHT_RELEVANCE = thoughtInfo.second;
}

std::string Data_Compiler::printThoughts() {
	std::string toReturn = "";
	for (int i = 0; i < THOUGHT_RELEVANCE.size(); i++) {
		toReturn.append("Situation: " + THOUGHTS[i].situation.subject + "," + THOUGHTS[i].situation.trait + "," + THOUGHTS[i].situation.value + "\n");
		toReturn.append("Action: " + THOUGHTS[i].action + "\n");
		toReturn.append("Rating: " + std::to_string(THOUGHTS[i].rating) + "\n");
		toReturn.append("\tTimes thought: " + std::to_string(THOUGHT_RELEVANCE[i]) + "\n\n");
	}
	return toReturn;
}


void Data_Compiler::exportThoughts() {
	std::ofstream thoughtOutput;
	thoughtOutput.open("thoughts.csv");

	for (int i = 0; i < THOUGHT_RELEVANCE.size(); i++) {
		int subject, trait, value, action, rating, relevance;
		if (THOUGHTS[i].situation.subject == "Org") { subject = 0; }
		else { subject = 7; }

		if (THOUGHTS[i].situation.trait == "Mateable") { trait = 0; }
		else if (THOUGHTS[i].situation.trait == "Proximity") { trait = 1; }
		else if (THOUGHTS[i].situation.trait == "Color") { trait = 3; }
		else if (THOUGHTS[i].situation.trait == "Size") { trait = 5; }
		else if (THOUGHTS[i].situation.trait == "Newborn") { trait = 7; }

		value = std::stoi(THOUGHTS[i].situation.value);

		if (THOUGHTS[i].action == "Idle") { trait = 0; }
		else if (THOUGHTS[i].action == "Eat") { trait = 1; }
		else if (THOUGHTS[i].action == "Attack") { trait = 3; }
		else if (THOUGHTS[i].action == "MoveAway") { trait = 4; }
		else if (THOUGHTS[i].action == "MoveToward") { trait = 5; }
		else if (THOUGHTS[i].action == "MatingOn") { trait = 6; }
		else if (THOUGHTS[i].action == "MatingOff") { trait = 7; }

		rating = THOUGHTS[i].rating;

		relevance = THOUGHT_RELEVANCE[i];

		thoughtOutput << subject << "," << trait << "," << value << "," << rating << "," << relevance << std::endl;
	}
	thoughtOutput.close();
}