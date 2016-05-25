#include <random>
#include <string>
#include <vector>
#include <iostream>
#include <time.h>
#include "Server.h"
#include "DataCompiler.h"

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

void stepByStep(Server* server, std::string input) {
	std::cout << server->printOrgList() << std::endl;
	std::cout << server->printEnviron() << std::endl;
	std::cout << "Enter 'end' to end. Enter anything else to continue...\n";
	std::cin >> input;
}


int main(){
	int SEED = time(NULL);
	srand(SEED);

	int WIDTH = 10;
	int HEIGHT = 10;
	int INIT_SIZE = 100;
	int ITERATIONS = 100;
	int GENOME_LENGTH = 10;

	Server* server = new Server(WIDTH, HEIGHT, INIT_SIZE, GENOME_LENGTH);
	Data_Compiler* dataCompiler = new Data_Compiler(server);

	//for (int i = 0; i < INIT_SIZE; i++){
	//	spawnNewOrg(server, WIDTH, HEIGHT);
	//}


	std::string dna1 = "000100" + generateIntel(0, 1, 7, 5, 7);
	std::string dna2 = "000100" + generateIntel(0, 1, 0, 0, 7);
	//server->addOrg(3, 3, dna1);
	//server->addOrg(2, 3, dna2);
	//server->addOrg(3, 1, dna2);
	//server->addOrg(0, 2, dna2);
	//server->addOrg(3, 3, dna2);

	

	std::string input;
	bool isLife = true;
	int iterationOfDeath = -1;
	//while (input != "end"){
	for (int i = 0; i < ITERATIONS; i++){
		isLife = server->org_update();
		if (!isLife) {
			iterationOfDeath = i;
			break;
		}
		server->server_update();
		dataCompiler->updateInfo();

		//stepByStep(server, input);
		std::cout << "Completed iteration " << i << std::endl;
	}
	//std::cout << dataCompiler->printThoughts() << std::endl;
	if (iterationOfDeath > 0) {
		std::cout << std::endl;
		std::cout << "All life has died out." << std::endl;
		std::cout << "Iterations survived: " << iterationOfDeath << std::endl;
	}
	dataCompiler->exportThoughts();
	std::cout << "Exported thought info to 'thoughts.csv'." << std::endl;
	dataCompiler->exportStates();
	std::cout << "Exported state info to 'states.csv'." << std::endl;
	dataCompiler->exportActions();
	std::cout << "Exported action counts to 'actions.csv'." << std::endl;
	std::cout << "Press any key to close...";
	std::cin >> input;
	

	return 0;
}