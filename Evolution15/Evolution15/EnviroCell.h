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
	void insertOrg(int id);
	void removeOrg(int id);


	std::map<std::string, int> getResources();
	int getResource(std::string resource);
	void modifyResources(std::string resource, int delta);


private:
	std::vector<int> orgsPresent;

	//Mapping of resource names to amount of that resource
	std::map<std::string, int> resources = { { "Food", 0 } };

};



#endif //ENVIRONMENT_CELL_H