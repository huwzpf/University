#pragma once
#include "CPN_Transition.h"
class CPN_Infection_Transition : public CPN_Transition
{
public :
	CPN_Infection_Transition(double* in, double* out, double param) : 
		CPN_Transition( in,  out,  param) {}
	void calc(double dt);
	
};

