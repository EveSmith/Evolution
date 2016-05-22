#ifndef DATACOMPILER_H
#define DATACOMPILER_H

#include <map>
#include <string>
#include "Messages.h"
#include "Server.h"


class Data_Compiler {
public:
	Data_Compiler(Server* s);
	~Data_Compiler();

	void updateInfo();

	std::string printThoughts();
	void exportThoughts();
	void exportStates();
private:
	Server* server;
	std::vector<State> STATES;
	std::vector<Intel> THOUGHTS;
	std::vector<int> THOUGHT_RELEVANCE;
	std::vector<std::pair<Intel, int>> MOST_ACTED_ON;
};



#endif //DATACOMPILER_H