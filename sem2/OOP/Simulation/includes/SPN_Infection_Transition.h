#pragma once
#include "SPN_Transition.h"
class SPN_Infection_Transition: public SPN_Transition
{
public:
	SPN_Infection_Transition(int* in, int* out, double param) : SPN_Transition(in, out, param) {
		update_lambda();
	}
	void update_lambda();
};

