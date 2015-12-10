#ifndef MESSAGES_H
#define MESSAGES_H

#include <vector>

//Request server for knowledge of surroundings
struct SensoryRequest{
	int perception; //how many cells to give info about
};

//Type sent from organism to server to request some action.
struct OrgUpdate{
	int senderID; //Unique ID number
	bool alive = true; //Is the org dead or not
	int oldX, oldY; //Previous position (for finding purposes)
	int newX, newY; //Desired new position
	SensoryRequest sensoryrequest;
};



//Organism's perception of other organisms
struct OrgSense{
	//TO DO: ACTUALLY LET ORGS PERCEIVE STUFF ABOUT OTHER ORGS
	int ID;
};

//Organism knowledge about surrounding environment cells
struct CellSense{
	int food;
	std::vector<OrgSense> orgs;
};


//Type sent from server to organisms to update them.
struct ServerUpdate{
	bool checked = false;
	int newX, newY;
	std::vector<CellSense> surroundings;
};

#endif //MESSAGES_H