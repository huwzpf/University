#pragma once
#include "CPN_Infection_Transition.h"
#include "CPN_Positive_Transition.h"
#include <iostream>
#include <vector>
using namespace std;

class CPN
{
private:
	double initial_time;
	double end_time;
	double dt;

	double Infection_rate;
	double Recovery_rate;
	double Vaccinated_Infection_rate;
	double Vaccination_rate;

	double S;
	double I;
	double R;
	double V;

	vector<CPN_Transition*> transitions;
	CPN_Infection_Transition* infection;
	CPN_Positive_Transition* recovery;
	CPN_Positive_Transition* vaccination;
	CPN_Infection_Transition* vaccinated_infection;
public:
	CPN(double initial, double end, double dt, double A, double B, double V_rate, double V_inf, double S, double I, double R, double V);
	void simulate();

};

