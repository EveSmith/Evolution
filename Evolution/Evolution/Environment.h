#ifndef ENVIRONMENT
#define ENVIRONMENT

#include "EnviroCell.h"


class Envir{
public:
	Envir(int WIDTH, int HEIGHT);

	void resize(int NEW_WIDTH, int NEW_HEIGHT);

	void display();


private:
	Cell* matrix;
};



#endif //ENVIRONMENT