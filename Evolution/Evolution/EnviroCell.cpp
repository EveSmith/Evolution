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
std::string Cell::printInfoList(){
	std::string info = "Value: " + std::to_string(value)+"\nOrgs Present:";
	for (int i = 0; i < orgsPresent.size(); i++){
		info.append("\t"+std::to_string(orgsPresent[i])+"\n");
	}
	return info;
}

std::string Cell::printInfoGrid(){
	std::string info = std::to_string(int(value)) + ":";
	if (!orgsPresent.empty()){
		info.append(std::to_string(orgsPresent.size()));
	}
	else{
		info.append(" ");
	}
	return info;
}



float Cell::getValue(){
	return value;
}

//Indicates whether org is present in cell
bool Cell::containsOrg(int id){
	return (find(orgsPresent.begin(), orgsPresent.end(), id) != orgsPresent.end());
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