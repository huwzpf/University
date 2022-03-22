#pragma once
#include <vector>
#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include <fstream>
#include <Windows.h>
#include <conio.h>
#include <cstdio>

using namespace std;


#define DEF_MOVING_RANGE 1
#define DEF_MOVING_OPTIONS 3

#define HUMAN_INITIATIVE 4
#define HUMAN_STRENGHT 5
#define ABILITY_COOLDOWN 5
#define ABILITY_DURTION 5
#define HUMAN_CHARACTER 'X'
#define HUMAN_RANGE 1
#define TURNS_WITH_HALF_CHANCE_TO_ACTIVATE 2
#define CHANCE_FOR_ABILITY_TO_FAIL 0.5

#define SHEEP_GENERATION_PROBABILITY 2
#define SHEEP_INITIATIVE 4
#define SHEEP_CHARACTER 'S'
#define SHEEP_STRENGHT 4
#define SHEEP_RANGE 1


#define WOLF_GENERATION_PROBABILITY 2
#define WOLF_STRENGHT 9
#define WOLF_INITIATIVE 5
#define WOLF_CHARACTER 'W'
#define WOLF_RANGE 1


#define TORTOISE_GENERATION_PROBABILITY 20
#define TORTOISE_STRENGHT 2
#define TORTOISE_INITIATIVE 1
#define TORTOISE_CHARACTER 'T'
#define TORTOISE_RANGE 1
#define TORTOISE_PROBABILITY_TO_MOVE 0.25
#define TORTOISE_DEFENSE_CAPABILITY 5


#define FOX_GENERATION_PROBABILITY 2
#define FOX_STRENGHT 3
#define FOX_INITIATIVE 7
#define FOX_CHARACTER 'F'
#define FOX_RANGE 1


#define ANTELOPE_GENERATION_PROBABILITY 2
#define ANTELOPE_STRENGHT 4
#define ANTELOPE_INITIATIVE 4
#define ANTELOPE_CHARACTER 'A'
#define ANTELOPE_RANGE 2
#define ANTELOPE_PROBABLITY_TO_ESCAPE 0.5


#define DANDELION_GENERATION_PROBABILITY 2
#define DANDELION_STRENGHT 0
#define DANDELION_CHARACTER '*'
#define DANDELION_SPREAD_RATE 0.1
#define DANDELION_SPREAD_ATTEMPTS 3


#define GRASS_GENERATION_PROBABILITY 2
#define GRASS_SPREAD_RATE 0.1
#define GRASS_STRENGHT 0
#define GRASS_CHARACTER ','

#define GUARANA_GENERATION_PROBABILITY 1
#define GUARANA_SPREAD_RATE 0.1
#define GUARANA_STRENGHT 0
#define GUARANA_CHARACTER '+'
#define GUARANA_EATING_BOOST 3

#define WOLFBERRY_GENERATION_PROBABILITY 1
#define WOLFBERRY_STRENGHT 0
#define WOLFBERRY_CHARACTER '?'
#define WOLFBERRY_SPREAD_RATE 0.1


#define HOGWEED_GENERATION_PROBABILITY 1
#define HOGWEED_SPREAD_RATE 0.05
#define HOGWEED_STRENGHT 10
#define HOGWEED_CHARACTER '|'
#define POISON_RADIUS 1


#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77



struct generation_probability {
	const char* organism_type;
	double probability;
};



enum collision_result {
	KILLED_OPPONENT,
	DIED,
	BOTH_DIED,
	ATTACKER_ESCAPED,
	DEFENDER_ESCAPED,
	BREEDED
};

enum action_result {
	MOVED,
	STAYED,
	SPREADED,
	BRUTALLY_MURDERED_EVERYTING_AROUND
};

enum direction {
	UP=0,
	DOWN=1,
	LEFT=2,
	RIGHT=3
};

struct coordinates {
	int x;
	int y;
	friend bool operator== (coordinates a, coordinates b) {
		if (a.x == b.x && a.y == b.y)
			return true;
		else return false;
	}

	friend bool operator!= (coordinates a, coordinates b) {
		return !(a == b);
	}

	string convert_to_string() {
		return( "( " + to_string(x) + "," + to_string(y) + " )");
	}
};

