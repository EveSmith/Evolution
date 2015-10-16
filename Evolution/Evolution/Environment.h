#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <string>
#include <vector>
#include "EnviroCell.h"


class Environment{
public:
	Environment(int WIDTH, int HEIGHT);
	~Environment();

	std::string print();

private:
	int width;
	int height;
	int size;
	std::vector<Cell*> matrix;
};



#endif //ENVIRONMENT_H