#ifndef ENVIRONMENT_CELL_H
#define ENVIRONMENT_CELL_H

#include <vector>
#include <string>


class Cell{
public:
	Cell();
	Cell(float VALUE);

	std::string printInfo();
	int getValue();

	void insertOrg(int org);
	void removeOrg(int org);


private:
	float value;
	std::vector<int> orgsPresent;

};



#endif //ENVIRONMENT_CELL_H