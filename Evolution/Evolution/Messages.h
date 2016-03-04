#ifndef MESSAGES_H
#define MESSAGES_H

#include <vector>
#include <map>
#include <array>


//Type sent from organism to server to request some action.
struct OrgUpdate{
	int senderID; //Unique ID number
	int oldX, oldY; //Previous position (for finding purposes)

	bool alive = true; //Is the org dead or not

	std::string action;
	int newX, newY; //Desired new position
	int targetID; //ID of attack victim
	int amount; //Amount to eat or to attack
};



//Organism's perception of other organisms
struct OrgSense{
	int ID = -1;
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
	bool operator==(const Situation& a) const {
		return (a.subject == this->subject && a.trait == this->trait && a.value == this->value);
	}
	std::string subject;
	std::string trait;
	std::string value;
};

//Individual units of knowledge: associations between observations and actions
struct Intel {
	bool operator==(const Intel& a) const {
		if (a.action == this->action && a.rating == this->rating) {
			if (a.situation == this->situation) {
				return true;
			}
		}
		return false;
	}
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
	int Age = 0;
};


//Type sent from server to organisms to update them.
struct ServerUpdate {
	bool checked;
	int newX, newY;
	Surroundings surroundings;
};

#endif //MESSAGES_H