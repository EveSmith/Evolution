#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <string>
#include <vector>
#include "EnviroCell.h"


class Environment{
public:
	Environment(int WIDTH, int HEIGHT);
	~Environment();

	std::string printList();
	std::string printGrid();

private:
	int width;
	int height;
	int size;
	std::vector<Cell*> matrix;
};



#endif //ENVIRONMENT_H