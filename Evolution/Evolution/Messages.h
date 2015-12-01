#ifndef MESSAGES_H
#define MESSAGES_H


struct AttackRequest{
	bool attacking;
	int victimID;
	int damageDone;
};

struct EatRequest{
	bool eating;
	int posX, posY;

};

struct MateRequest{
	bool mating;
	int mateID;
};

//Type sent from organism to server to request some action.
struct OrgUpdate{
	int senderID; //Unique ID number
	int oldX, oldY; //Previous position (for finding purposes)
	int newX, newY; //Desired new position
	int health;
	AttackRequest attack;
	EatRequest eat;
	MateRequest mate;
};



//Type sent from server to organisms to update them.
struct ServerUpdate{
	bool checked;
	int newX, newY;
};

#endif //MESSAGES_H