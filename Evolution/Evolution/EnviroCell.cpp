#include "EnviroCell.h"

#include <algorithm>

Cell::Cell(){
	value = rand() % 10;
}

Cell::Cell(float VALUE){
	value = VALUE;
}


/**returns a string representation of cell:
Value: <value>
Orgs Present:
	1234
	5678
*/
std::string Cell::printInfo(){
	std::string info = "Value: " + std::to_string(value)+"\nOrgs Present:";
	for (int i = 0; i < orgsPresent.size(); i++){
		info.append("\t"+std::to_string(orgsPresent[i])+"\n");
	}
	return info;
}

int Cell::getValue(){
	return value;
}

void Cell::insertOrg(int org){
	std::vector<int>::iterator position = find(orgsPresent.begin(), orgsPresent.end(), org);
	if ( position == orgsPresent.end()){
		orgsPresent.push_back(org);
	}
}

void Cell::removeOrg(int org){
	std::vector<int>::iterator position = find(orgsPresent.begin(), orgsPresent.end(), org);
	if (position == orgsPresent.end()){
		orgsPresent.erase(position);
	}
}