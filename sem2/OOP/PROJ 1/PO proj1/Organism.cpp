#include "Organism.h"

bool Organism::operator < (const Organism& b) const { 
	if (initiative != b.initiative)
		return initiative > b.initiative;
	else //if organisms have the same initiative the older one (creater earlier) should be first in vector 
		return creation_turn < b.creation_turn;
}
