#ifndef ENVIRONMENT_CELL_H
#define ENVIRONMENT_CELL_H

#include <vector>
#include <map>
#include <string>


class Cell{
public:
	Cell();

	std::string printInfoList();
	std::string printInfoGrid();

	bool containsOrg(int id);
	std::vector<int> getOrgs();
	int countOrgs();
	void insertOrg(int id);
	void removeOrg(int id);

	int getFood();
	void modifyFood(int delta);
	void randomReplenish();


private:
	std::vector<int> orgsPresent;
	int food;
	int replenishRate;

};



#endif //ENVIRONMENT_CELL_H