#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <string>
#include <vector>
#include <map>
#include "EnviroCell.h"


class Environment{
public:
	Environment(int WIDTH, int HEIGHT);
	~Environment();

	std::string printList();
	std::string printGrid();

	int findOrg(int id);

	void addOrg(int id, int x, int y);
	void remOrg(int id);
	void remOrg(int id, int x, int y);
	void moveOrg(int id, int oldX, int oldY, int newX, int newY);
	std::vector<int> getOrgs(int x, int y);
	std::vector<int> getOrgs(int index);

	std::map<std::string, int> getResources(int x, int y);
	int getResources(int x, int y, std::string resource);
	void changeResources(int x, int y, std::map<std::string, int> deltaMap);
	void changeResources(int x, int y, std::string resource, int delta);

private:
	int width;
	int height;
	int size;
	std::vector<Cell*> matrix;
};



#endif //ENVIRONMENT_H