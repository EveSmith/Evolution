#include <random>
#include <string>
#include <iostream>
#include "Environment.h"

using namespace std;

int main(){
	int SEED = 0;
	srand(SEED);


	Environment environment = Environment(5, 5);


	cout << environment.print();

	cout << "Press any key...";
	cin.get();

	return 0;
}