#ifndef MESSAGES_H
#define MESSAGES_H

//Type sent from organism to server to request some action.
struct OrgUpdate{
	int senderID; //Unique ID number
	int oldX, oldY; //Previous position (for finding purposes)
	int newX, newY; //Desired new position
	int health;
};

//Type sent from server to organisms to update them.
struct ServerUpdate{
	bool checked;
	int newX, newY;
};

#endif //MESSAGES_H