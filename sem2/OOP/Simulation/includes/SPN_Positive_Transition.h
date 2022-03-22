#pragma once
#include "SPN_Transition.h"
class SPN_Positive_Transition : public SPN_Transition
{
public:
	SPN_Positive_Transition(int* in, int* out, double param) : SPN_Transition(in, out, param) {
		update_lambda();
	}
	void update_lambda();
};

