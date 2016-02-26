#ifndef DATACOMPILER_H
#define DATACOMPILER_H

#include <map>
#include <string>
#include "Messages.h"

std::vector<Intel> THOUGHTS;
std::vector<int> THOUGHT_RELEVANCE;
std::vector<std::pair<Intel, int>> MOST_ACTED_ON;

std::string printThoughts() {
	std::string toReturn = "";
	for (int i = 0; i < THOUGHT_RELEVANCE.size(); i++) {
		toReturn.append("Situation: " + THOUGHTS[i].situation.subject + "," + THOUGHTS[i].situation.trait + "," + THOUGHTS[i].situation.value + "\n");
		toReturn.append("Action: " + THOUGHTS[i].action + "\n");
		toReturn.append("Rating: " + std::to_string(THOUGHTS[i].rating) + "\n");
		toReturn.append("\tTimes thought: " + std::to_string(THOUGHT_RELEVANCE[i]) + "\n\n");
	}
	return toReturn;
}


#endif //DATACOMPILER_H