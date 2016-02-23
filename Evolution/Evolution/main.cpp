#include <random>
#include <string>
#include <vector>
#include <iostream>
#include <time.h>
#include "Server.h"

std::string int_to_bin(int i) {
	std::string bin = "";
	if (i >= 4) { bin.append("1"); }
	else { bin.append("0"); }
	if (i % 4 >= 2) { bin.append("1"); }
	else { bin.append("0"); }
	if (i % 2 >= 1) { bin.append("1"); }
	else { bin.append("0"); }
	return bin;
}

std::string generateIntel(int subject, int trait, int value, int action, int rating) {
	std::string intel= "";
	intel.append(int_to_bin(subject));
	intel.append(int_to_bin(trait));
	intel.append(int_to_bin(value));
	intel.append(int_to_bin(action));
	intel.append(int_to_bin(rating));
	return intel;
}


int main(){
	int SEED = 0;//time(NULL);
	srand(SEED);

	int WIDTH = 5;
	int HEIGHT = 5;
	int INIT_SIZE = 0;

	Server* server = new Server(WIDTH, HEIGHT, INIT_SIZE);

	//for (int i = 0; i < INIT_SIZE; i++){
	//	spawnNewOrg(server, WIDTH, HEIGHT);
	//}

	std::string dna1 = "000000" + generateIntel(7, 1, 0, 0, 7); //If food nearby, ignore
	std::string dna2 = "000000" + generateIntel(0, 1, 0, 3, 7); //If org nearby, attack
	std::string dna3 = "000000000";
	server->addOrg(1, 1, dna1); 
	server->addOrg(1, 1, dna2);

	

	std::string input;
	while (input != "end"){
		//TO DO: CHANGE TO "SIMULTANEOUS" BEHAVIOR
		server->org_update();
		server->server_update();

		std::cout << server->printOrgList();
		std::cout << server->printEnviron();

		std::cout << "Enter 'end' to end. Enter anything else to continue...\n";
		std::cin >> input;
	}

	return 0;
}