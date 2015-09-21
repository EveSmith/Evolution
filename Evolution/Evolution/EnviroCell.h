#ifndef ENVIRONMENT_CELL
#define ENVIRONMENT_CELL

#include <vector>


class Cell{
public:
	Cell();

private:
	int value;
	std::vector<int> orgsPresent;

};



#endif //ENVIRONMENT_CELL