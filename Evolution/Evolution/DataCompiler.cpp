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
		thoughtOutput << THOUGHTS[i].situation.subject << ",";
		thoughtOutput << THOUGHTS[i].situation.trait << ",";
		thoughtOutput << THOUGHTS[i].situation.value << ",";
		thoughtOutput << THOUGHTS[i].rating << ",";
		thoughtOutput << THOUGHT_RELEVANCE[i] << std::endl;
	}
	thoughtOutput.close();
}