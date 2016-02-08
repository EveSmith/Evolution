#ifndef MESSAGES_H
#define MESSAGES_H

#include <vector>
#include <map>

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
};



//Organism's perception of other organisms
struct OrgSense{
	int ID;
	bool mateable;
	std::array<int, 2> position; //relative position from self
	int color;
	int size;
	bool newborn;
};

struct Surroundings {
	std::vector<std::pair<std::array<int, 2>, OrgSense>> orgsNearby; //<Relative location from self, Org sense>
	std::vector <std::pair<std::array<int, 2>, int>> foodNearby; //<Relative location from self, food amount>
};


//Situation
struct Situation {
	std::string subject;
	std::string trait;
	std::string value;
};

//Individual units of knowledge: associations between observations and actions
struct Intel {
	Situation situation;
	std::string action;
	int rating;
};

struct Traits {
	int Health;
	int Color;
	int Size;
	bool Mateable;
	bool Newborn;
};


//Type sent from server to organisms to update them.
struct ServerUpdate {
	bool checked;
	int newX, newY;
	Surroundings surroundings;
};

struct Params {
	int deltaX;
	int deltaY;
	int targetID;
};

#endif //MESSAGES_H