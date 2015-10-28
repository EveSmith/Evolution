#include <random>
#include <string>
#include <iostream>
#include "Server.h"

using namespace std;

int main(){
	int SEED = 0;
	srand(SEED);


	Server server = Server(5, 5, 5);


	cout << server.printEnviron();
	cout << server.printOrgList();

	cout << "Press any key...";
	cin.get();

	return 0;
}