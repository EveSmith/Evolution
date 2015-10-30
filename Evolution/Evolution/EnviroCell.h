#ifndef ENVIRONMENT_CELL_H
#define ENVIRONMENT_CELL_H

#include <vector>
#include <string>


class Cell{
public:
	Cell();
	Cell(float VALUE);

	std::string printInfoList();
	std::string printInfoGrid();

	float getValue();
	bool containsOrg(int id);

	void insertOrg(int id);
	void removeOrg(int id);


private:
	float value;
	std::vector<int> orgsPresent;

};



#endif //ENVIRONMENT_CELL_H