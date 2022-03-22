
#include "includes/CPN.h"
#include "includes/SPN.h"

using namespace std;

int main() {
	int sim;
	int Suspected=999;
	int Infected=1;
	int Recovered=0;
	int Vaccinated=0;
	double Vaccination_rate = 0.01;
	double Vaccine_efficiency = 0.85;
	double Days_to_recover = 10;
	double Days_to_infect = 4;
	int time = 200;

	/*
	cout << "Podaj parametry symulacji :" << endl;
	cout << endl << "Liczba osób zdrowych : ";
	cin >> Suspected;
	cout << endl << "Liczba osób chorych : ";
	cin >> Infected;
	cout << endl << "Liczba osób wyzdrowialych : ";
	cin >> Recovered;
	cout << endl << "Liczba osób zaszczepionych : ";
	cin >> Vaccinated; 
	cout << endl << "Jaki procent osob jest szczepionych dziennie : ";
	cin >> Vaccination_rate;
	cout << endl << "Jaka jest skutecznosc szczepionki : ";
	cin >> Vaccine_efficiency;
	cout << endl << "Ile dni zajmuje wyzdrowienie : ";
	cin >> Days_to_recover;
	cout << endl << "Co ile dni srednio zarazana jest kolejna osoba : ";
	cin >> Days_to_infect;
	cout << endl << "Ile czasu ma trwac symulacja : ";
	cin >> time;
	*/
	



	cout << "Podaj metode symulacji: (1-CPN, 2-SPN) ";
	cin >> sim;
	if (sim == 1) {
		CPN* CPN_simulation = new CPN(0, time, 1, (1.0/Days_to_infect), (1.0/Days_to_recover), Vaccination_rate, (1-Vaccine_efficiency), Suspected, Infected,  Recovered, Vaccinated);
		CPN_simulation->simulate();
		delete CPN_simulation;
		
	}
	else if (sim == 2) {
		SPN* SPN_simulation = new SPN(0, time, (1.0 / Days_to_infect), (1.0 / Days_to_recover), Vaccination_rate, (1 - Vaccine_efficiency), Suspected, Infected, Recovered, Vaccinated);
		SPN_simulation->simulate();
		delete SPN_simulation;
	}

	
		return 0;

}
