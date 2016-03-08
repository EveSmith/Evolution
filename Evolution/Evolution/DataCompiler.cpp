#include <fstream>
#include "DataCompiler.h"


Data_Compiler::Data_Compiler(Server* s) {
	server = s;
}

void Data_Compiler::updateInfo() {
	std::pair<std::vector<Intel>, std::vector<int>> thoughtInfo = server->getThoughtRelevancies();
	//For every thought that was thought(?) last iteration...
	for (int i = 0; i < thoughtInfo.first.size(); i++) {
		//Does THOUGHTS already have that thought?
		auto it = std::find(THOUGHTS.begin(), THOUGHTS.end(), thoughtInfo.first[i]);
		//If yes...
		if (it != THOUGHTS.end()) {
			//Add new relevance to that thought
			THOUGHT_RELEVANCE[it - THOUGHTS.begin()] += thoughtInfo.second[i];
		}
		//If not...
		else {
			//Append that thought and relevancy to THOUGHTS
			THOUGHTS.push_back(thoughtInfo.first[i]);
			THOUGHT_RELEVANCE.push_back(thoughtInfo.second[i]);
		}
	}

	STATES.push_back(server->getState());
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
	thoughtOutput << "Subject,Trait,Value,Action,Rating,Relevance" << std::endl;
	for (int i = 0; i < THOUGHT_RELEVANCE.size(); i++) {
		thoughtOutput << THOUGHTS[i].situation.subject << ",";
		thoughtOutput << THOUGHTS[i].situation.trait << ",";
		thoughtOutput << THOUGHTS[i].situation.value << ",";
		thoughtOutput << THOUGHTS[i].action << ",";
		thoughtOutput << THOUGHTS[i].rating << ",";
		thoughtOutput << THOUGHT_RELEVANCE[i] << std::endl;
	}
	thoughtOutput.close();
}

void Data_Compiler::exportStates() {
	std::ofstream stateOutput;
	stateOutput.open("states.csv");
	stateOutput << "Population,AverageHealth" << std::endl;
	for (int i = 0; i < STATES.size(); i++) {
		stateOutput << STATES[i].population << "," << STATES[i].avgHealth << std::endl;
	}
	stateOutput.close();
}