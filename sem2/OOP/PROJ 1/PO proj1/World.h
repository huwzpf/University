#pragma once
#include "GeneralHeader.h"
#include "Organism.h"

using namespace std;

class Organism;

class World
{
private:

	int width;
	int height;
	int turn_counter;
	int organisms_count;
	vector<string> turn_log;
	fstream game_log;

	Organism* human;

	mt19937 rand_engine;
	uniform_real_distribution<> utility_real_distribution_0_1;

	vector<Organism*> organisms; //vector of organisms to make it easier to update every organism without iterating through whole map
	Organism*** Map; //2D array of Organism pointers

	Organism** generate_random_organisms(int n);
	//functions for random organism generation
	Organism* generate_organism(const char* c, coordinates position);
	vector<generation_probability> probabilities_vector;
	void populate_probabilities_vector();
	
	
	bool check_game_state_validity();

	void move_organism(Organism* o, coordinates new_position);
	void draw_world();
	void execute_turn();
	void save_to_game_log();
	void handle_escape(coordinates moved_pos);
	void populate_world(int number);
	void add_organism(Organism*, coordinates position);
	void kill_everything_around(coordinates position);
	void handle_move(Organism* organism, coordinates moved_pos);
	void breed_organisms(coordinates moved_pos, Organism* organism);
	void remove_organism(Organism* o);

	
	void load_game();

public:

	World(int w, int h);
	
	~World();

	void simulate();
	void log(string s);
	void save_game();

	coordinates find_free_adjacent(coordinates base, int range, bool search_wider_if_not_found) const;

	

	//getters 
	double get_random_number() {
		return  utility_real_distribution_0_1(rand_engine);
	}

	Organism* get_map_field(int x, int y) const {
		return Map[x][y];
	}
	
	int get_turn() const {
		return turn_counter;
	}

	int get_height() const {
		return height;
	}

	int get_width() const {
		return width;
	}


};

