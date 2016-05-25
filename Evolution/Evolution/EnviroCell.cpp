#include "EnviroCell.h"

#include <algorithm>
#include <stdexcept>

Cell::Cell(){
	food = rand() % 10;
	replenishRate = 5 + (rand() % 10);
}


/**returns a string representation of cell:
Value: <value>
Orgs Present:
	1234
	5678
*/
std::string Cell::printInfoList(){
	std::string info = "Orgs Present:";
	for (int i = 0; i < orgsPresent.size(); i++){
		info.append("\t"+std::to_string(orgsPresent[i])+"\n");
	}
	return info;
}

std::string Cell::printInfoGrid(){
	std::string info;
	if (!orgsPresent.empty()){
		info = " " + std::to_string(orgsPresent.size()) + " ";
	}
	else{
		info.append("   ");
	}
	
	return info;
}


//Indicates whether org is present in cell
bool Cell::containsOrg(int id){
	return (find(orgsPresent.begin(), orgsPresent.end(), id) != orgsPresent.end());
}

//Returns list of orgs present in cell
std::vector<int> Cell::getOrgs() {
	return orgsPresent;
}

int Cell::countOrgs() {
	return orgsPresent.size();
}

//If org is not already present, add it.
void Cell::insertOrg(int id){
	std::vector<int>::iterator position = find(orgsPresent.begin(), orgsPresent.end(), id);
	if ( position == orgsPresent.end()){
		orgsPresent.push_back(id);
	}
}

//Finds specified org and removes it
void Cell::removeOrg(int id){
	std::vector<int>::iterator position = find(orgsPresent.begin(), orgsPresent.end(), id);
	if (position != orgsPresent.end()){
		orgsPresent.erase(position);
	}
}


//Returns this cell's resource map
int Cell::getFood(){
	return food;
}

//Changes amount of resource by the specified delta
void Cell::modifyFood(int delta){
	food += delta;
	if (food < 0) {
		food = 0;
	}
}

void Cell::randomReplenish() {
	if (rand() % replenishRate == 0) {
		food += 3;
	}
}