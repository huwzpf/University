#include <cstdlib>
#include <iostream>

#include "World.h"
using namespace std;

int main() {
	int x;
	int y;
	cout << "X:" << endl;
	cin >> x;
	cout << "Y:" << endl;
	cin >> y;
	World* w = new World(x,y);
	w->simulate();
	
	return 0;

}