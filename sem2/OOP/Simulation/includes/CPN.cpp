#include "CPN.h"
CPN::CPN(double initial, double end, double dt, double I_rate, double R_rate, double V_rate, double V_inf, double s, double i, double r, double v) :
	dt(dt), initial_time(initial), end_time(end),  S(s), I(i), R(r), V(v) {

	int population = i + r + v + s;
	Infection_rate = I_rate / population;
	Recovery_rate = R_rate;
	Vaccination_rate = V_rate;
	Vaccinated_Infection_rate = V_inf * Infection_rate;


	infection = new CPN_Infection_Transition(&S, &I, Infection_rate);
	transitions.push_back(infection);

	vaccinated_infection = new CPN_Infection_Transition(&V, &I, Vaccinated_Infection_rate);
	transitions.push_back(vaccinated_infection);

	recovery = new CPN_Positive_Transition(&I, &R, Recovery_rate);
	transitions.push_back(recovery);

	vaccination = new CPN_Positive_Transition(&S, &V, Vaccination_rate);
	transitions.push_back(vaccination);

}


void CPN::simulate() {
	int values;
	cout << "Display exact values - 1 or rounded ones - 2 ?" << endl;
	cin >> values;

	double time = initial_time;

	while (time < end_time && (time>0? round(I)>0:1)) {
		time += dt;
		for (auto t : transitions) {
			t->calc(dt); // calculate change made by transition t
		}
		for (auto t : transitions) {
			t->update(); // update data based on delta values calculated by calc()
		}
		if(values==1)
		cout << "EXACT VALUES" << "--Time: " << time << " days " << "Healthy: " << S << ", Infected: " << I << ", Recovered: " << R << ", Vaccinated: " << V << endl;
		else
		cout << "ROUNDED VALUES" << "--Time: " << time << " days " << "Healthy: " << round(S) << ", Infected: " << round(I) << ", Recovered: " << round(R) << ", Vaccinated: " << round(V) << endl;
	}


}