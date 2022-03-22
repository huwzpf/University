#pragma once
#include "SPN_Infection_Transition.h"
#include "SPN_Positive_Transition.h"
#include <vector>
#include <random>
#include <iostream>
#include <algorithm>
using namespace std;
class SPN 
{
private:
	double initial_time;
	double end_time;
	double time;
	double Infection_rate;
	double Recovery_rate;
	double Vaccined_Infection_rate;
	double Vaccination_rate;
	int S;
	int I;
	int R;
	int V;
	vector<SPN_Transition*> transitions;
	SPN_Infection_Transition* infection;
	SPN_Positive_Transition* recovery;
	SPN_Positive_Transition* vaccination;
	SPN_Infection_Transition* vaccinated_infection;
public:
	SPN(double initial, double end, double A, double B, double V_rate, double V_inf, int S, int I, int R, int V);
	void simulate();
	double get_time() {
		return time;
	}

};

