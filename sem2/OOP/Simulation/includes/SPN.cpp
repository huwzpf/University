#include "SPN.h"

bool compare(SPN_Transition* a, SPN_Transition* b) {
	return (a->get_lambda() < b->get_lambda());
}

SPN::SPN(double initial, double end, double I_rate, double R_rate, double V_rate, double V_inf, int s, int i, int r, int v) :
	time(initial), initial_time(initial), end_time(end),  S(s), I(i), R(r), V(v) {

	int population = i + r + v + s;
	Infection_rate = I_rate / population;
	Recovery_rate = R_rate;
	Vaccination_rate = V_rate;
	Vaccined_Infection_rate = V_inf * Infection_rate;

	infection = new SPN_Infection_Transition(&S, &I, Infection_rate);
	transitions.push_back(infection);

	recovery = new SPN_Positive_Transition(&I, &R, Recovery_rate);
	transitions.push_back(recovery);

	vaccination = new SPN_Positive_Transition(&S, &V, Vaccination_rate);
	transitions.push_back(vaccination);

	vaccinated_infection = new SPN_Infection_Transition(&V, &I, Vaccined_Infection_rate);
	transitions.push_back(vaccinated_infection);

	sort(transitions.begin(), transitions.end(), compare);
}



void SPN::simulate() {

	std::random_device rd;
	std::mt19937 gen(rd()); //initialize random number generator

	double sum = 0;
	double lambda;
	while (time < end_time) { 
		lambda = 0; //sum of all lambdas
		for ( auto t : transitions) { 
			lambda += t->get_lambda();
		}
			
			exponential_distribution<double>  exponential(lambda); //initialize random distributions 
			uniform_real_distribution<> uniform(0.0, lambda);

			double rand = uniform(gen);
			for (auto t : transitions) {
				if (rand < t->get_lambda() + sum) { //based on uniform distribution choose which transition to make based on intervals - if random is between 0 and lambda1 choose lambda 1, between lambda 1 and lambda 2 choose lambda 2 etc.
					t->update();
					sum = 0;
					break;
				}
				else {
					sum += t->get_lambda();
				}
					
			}
			
			
			time += exponential(gen); //update time by random number from exponential distribution
			cout << "Time: " << time << " days " << "Healthy: " << S << ", Infected: " << I << ", Recovered: " << R << ", Vaccinated: " << V << endl;


		if(I==0) break;

	}

}