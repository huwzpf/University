#pragma once

class SPN_Transition
{
protected:

	int* in_connection;
	int* out_connection;
	double lambda;
	double parameter;
	virtual void update_lambda() {
		lambda = 0;
	}

public:
	SPN_Transition(int* in, int* out, double param) : in_connection(in), out_connection(out), parameter(param) {
		update_lambda();
	}
	virtual void shift();
	 double get_lambda() {
		 update_lambda();
		 return lambda;
	}
	 void update();
};

