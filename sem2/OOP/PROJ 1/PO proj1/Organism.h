#pragma once
#include "GeneralHeader.h"
#include "World.h"
using namespace std;
class World;




class Organism
{
protected:
	int initiative;
	int creation_turn;
	int strenght;
	int x_pos;
	int y_pos;
	World* world;
	const char* species_name;
	bool waiting;
	bool murders_everything_around;
public:
	
	//setters
	void set_position(coordinates c) {
		x_pos = c.x;
		y_pos = c.y;
	}

	void change_strenght(int change) {
		strenght += change;
	}

	void stop_waiting() {
		waiting = false;
	}

	

	//getters 
	bool does_murder_enrything_around() {
		return murders_everything_around;
	}

	bool is_waiting() {
		return waiting;
	}

	const char* get_species_name() {
		return species_name;
	}

	coordinates get_position() {
		return { x_pos, y_pos };
	}
	int get_strenght() {
		return strenght;
	}
	int get_age() {
		return creation_turn;
	}

	int get_initiative() {
		return initiative;
	}
	// basically stringify method , returns ' organism at (x,y) '
	string display() {
		return (species_name + (string)" at " + get_position().convert_to_string());
	}

	//constructor 
	Organism(int _initiative, int _strenght, int _creation_turn, coordinates _position, World* _world, const char* _name, bool _murders_everything_around):
	initiative(_initiative), strenght(_strenght), creation_turn(_creation_turn), x_pos(_position.x), y_pos(_position.y), world(_world), species_name(_name), murders_everything_around(_murders_everything_around)
	{
		waiting = true; // waitng flag starts set to true, so organisms do not take an action right after being created 
	}

	//all methods are purely virtual since they will differ in Animal and Plant calsses
	virtual void draw()=0;
	virtual action_result action()=0;
	virtual collision_result collision(Organism* opponent)=0;

	bool operator < (const Organism& b) const; // organisms shall be sorted in specific order in organisms vector, this overloaded operator ensures that
		
};

