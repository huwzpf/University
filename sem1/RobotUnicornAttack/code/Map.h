#pragma once
#include "Game.h"



class Map
{
public:

	Map();

	void load_map(int** arr);
	int** read_map();
	void allocate_objects_rect_arrays();
	void allocate_collision_array();
	void add_object(int row, int column, int** arr, int& counts, int& countf);
	void add_collision(int row, int column);



	void draw_map(int x, int y);
	void draw_borders();


	int get_map_rows() {
		return map_rows;
	}
	int get_map_cols() {
		return map_cols;
	}
	int get_level_width() {
		return level_width;
	}
	int get_level_height() {
		return level_height;
	}
	
	int** get_collision_cords() {
		return collision_cords;
	}

	SDL_Rect* get_stars_arr() {
		return stars_arr;
	}
	SDL_Rect* get_fairies_arr() {
		return fairies_arr;
	}

	int get_stars_number(){
		return stars;
	}
	int get_fairies_number(){
		return fairies;
	}

	bool did_error() { // bool indicating there was an error while loading from file 
		return error;
	}
	
private:
	SDL_Rect src, dest; //rects needed to draw map to screen
	SDL_Texture* floor;
	SDL_Texture* background;
	int** map;
	int map_rows, map_cols;
	int level_width, level_height;
	int** collision_cords;
	SDL_Rect* stars_arr; //arrays of objects positions
	SDL_Rect* fairies_arr;
	int stars, fairies;//object counts
	bool error = false;
	
};

