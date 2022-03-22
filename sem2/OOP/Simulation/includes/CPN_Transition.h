#pragma once
#include <cmath>
class CPN_Transition
{
protected:
	double* in_connection;
	double* out_connection;
	double parameter;
	double d_in;
	double d_out;
public:
	virtual void calc(double dt) = 0;
	void update();

	CPN_Transition(double* in, double* out, double param) : 
		in_connection(in), out_connection(out), parameter(param)		 {
		d_in = 0; d_out = 0;
	};
	
	
};

