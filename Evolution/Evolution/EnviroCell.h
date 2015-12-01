#ifndef ENVIRONMENT_CELL_H
#define ENVIRONMENT_CELL_H

#include <vector>
#include <string>


class Cell{
public:
	Cell();

	std::string printInfoList();
	std::string printInfoGrid();

	bool containsOrg(int id);

	void insertOrg(int id);
	void removeOrg(int id);


private:
	std::vector<int> orgsPresent;

};



#endif //ENVIRONMENT_CELL_H