#ifndef MESSAGES_H
#define MESSAGES_H

#include <vector>

struct EatRequest {
	bool request_made = false;
	int amount;
};
struct AttackRequest {
	bool request_made = false;
	int victimID;
	int strength;
};
struct MateRequest {
	bool request_made = false;
	int mateID;
};
struct HealRequest {
	bool request_made = false;
	int friendID;
	int strength;
};

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
	EatRequest eatrequest;
	AttackRequest attackrequest;
	MateRequest materequest;
	HealRequest healrequest;
};



//Organism's perception of other organisms
struct OrgSense{
	//TO DO: ACTUALLY LET ORGS PERCEIVE STUFF ABOUT OTHER ORGS
	int ID;
};

//Organism knowledge about surrounding environment cells
struct CellSense{
	int deltaX, deltaY; //difference in coords from org position
	int food;
	std::vector<OrgSense> orgs;
};


//Type sent from server to organisms to update them.
struct ServerUpdate{
	bool checked;
	int newX, newY;
	std::vector<CellSense> surroundings;
};

#endif //MESSAGES_H