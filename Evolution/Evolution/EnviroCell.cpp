#include "EnviroCell.h"

#include <algorithm>
#include <stdexcept>

Cell::Cell(){
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
std::map<std::string,int> Cell::getResources(){
	return resources;
}

//Returns the value of the specified resource
int Cell::getResource(std::string resource){
	if (resources.count(resource) > 0){
		return resources[resource];
	}
	else{
		throw std::invalid_argument("Resource " + resource + " does not exist");
	}
}

//Changes amount of resource by the specified delta
void Cell::modifyResources(std::string resource, int delta){
	if (resources.count(resource) > 0){
		resources[resource] += delta;
	}
	else{
		throw std::invalid_argument("Resource " + resource + " does not exist");
	}
}