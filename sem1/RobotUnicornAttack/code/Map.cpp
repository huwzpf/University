#include "Map.h"
#include "TextureManager.h"
#include <stdlib.h>
#include <time.h>

#define SQUARE_SIZE 60


int ** Map::read_map() {
	int** map;
	int rows, cols;
	char c;
	FILE* file;
	file = fopen("./map.txt", "r");
	fscanf(file, "rows number: %d , columns number: %d , stars number: %d , fairies number : %d", &rows, &cols, &stars, &fairies);
	map = (int**)malloc(rows * sizeof(int*));
	for (int i = 0; i < rows; i++) {
		c = getc(file);
		map[i] = (int*)malloc(cols * sizeof(int));
		for (int j = 0; j < cols; j++) {
			 c=getc(file);
			 if (c == ' ') { //skip spaces
				 j--;
				 continue;
			 }
			 else if (c == EOF || c=='\n') { // error detection
				 printf("wrong columns or rows number");
				 error = true;
			 }
			 map[i][j] = c - '0';
			 if (c-'0' < 0 || c - '0' >3) { // error detection
				 printf("incorrect symbol occured while loading map");
				 error = true;
			 }
		}
	}
	map_rows = rows;
	map_cols = cols;
	level_height = rows * SQUARE_SIZE;
	level_width = cols * SQUARE_SIZE;
	fclose(file);
	return map;
}


Map::Map() {
	floor = TextureManager::load_texture("./floor.bmp",false);
	background = TextureManager::load_texture("./background.bmp",false);
	stars_arr = NULL; //arrays of coordinates of every loaded object
	fairies_arr = NULL;

	load_map(read_map());

	src.x = src.y =dest.y = dest.x = 0; //coordinates needed to generate single map block
	src.w = src.h = SQUARE_SIZE;
	dest.w = dest.h = SQUARE_SIZE;
	
}


void Map::load_map(int** arr) {
	//allocate all needed memory blocks

	allocate_collision_array();

	map = (int**)malloc(map_rows * sizeof(int*));
	if (map == NULL) {
		printf("error allocating memory");
		error = true;
	}
	
	allocate_objects_rect_arrays();

	int counts = 0;
	int countf = 0;

	for (int row = 0; row < map_rows; row++) { // fills final array with numbers and fills in collision array and stuff related to objects like stars and fairies

		map[row] = (int*)malloc(map_cols * sizeof(int));
		if(map[row]==NULL){
			printf("error allocating memory");
			error = true;
			}

		for (int column = 0; column < map_cols; column++) {
			if (arr[row][column] < 2) {
				map[row][column] = arr[row][column];

				if (map[row][column] == 1) {
					add_collision(row, column);
				}
				}
			else if (arr[row][column] >= 2) {
				add_object(row, column, arr, counts, countf);
				
				}
			}
		}
	if (counts != stars || countf != fairies) { // detects errors
		printf("wrong fairies or stars number");
		error = true;
	}

}

void Map::allocate_objects_rect_arrays()
{
	stars_arr = (SDL_Rect*)malloc(sizeof(SDL_Rect) * stars);
	if (stars_arr == NULL) {
		printf("error");
	}
	fairies_arr = (SDL_Rect*)malloc(sizeof(SDL_Rect) * fairies);
	if (fairies_arr == NULL) {
		printf("error");
	}
}

void Map::allocate_collision_array()
{
	collision_cords = (int**)malloc(level_height * sizeof(int*));
	if (collision_cords == NULL) {
		printf("error allocating memory");
		error = true;
	}
	for (int i = 0; i < level_height; i++) {
		collision_cords[i] = (int*)malloc(level_width * sizeof(int));
		if (collision_cords[i] == NULL) {
			printf("error allocating memory");
			error = true;
		}
		for (int j = 0; j < level_width; j++) {
			collision_cords[i][j] = 0;
		}
	}
}

void Map::add_object(int row, int column, int** arr, int& counts, int& countf) //
{
	map[row][column] = 0; //objects like fairies arent part of map 


	if (arr[row][column] == 2) { //fills rects in corresponding arrays

		stars_arr[counts].y = row * SQUARE_SIZE;
		stars_arr[counts].x = column * SQUARE_SIZE;
		stars_arr[counts].w = stars_arr[counts].h = SQUARE_SIZE;
		counts++;

	}
	else if (arr[row][column] == 3) {

		fairies_arr[countf].y = row * SQUARE_SIZE;
		fairies_arr[countf].x = column * SQUARE_SIZE;
		fairies_arr[countf].w = fairies_arr[countf].h = SQUARE_SIZE;
		countf++;

	}
}

void Map::draw_map(int x, int y) {
	int type;

	for (int row = 0; row < map_rows; row++) {
		for (int column = 0; column < map_cols; column++) {
			type = map[row][column];
			dest.x = column * SQUARE_SIZE - x; // draws textures adjusted by camera coordinates
			dest.y = row * SQUARE_SIZE - y;

			switch (type) { //draws different texture depending on number in array
			case 0:
				TextureManager::draw(background, src, dest); 
				break;
			case 1:
				TextureManager::draw(floor, src, dest);
				
				break;
			default: 
				break;
			}
		}
	}
	draw_borders();//draws screen borders
}

void Map::draw_borders()
{
	SDL_Rect recta = { 0,0,5,SCREEN_WIDTH };
	SDL_Rect rectb = { SCREEN_WIDTH - 5,0,5,SCREEN_WIDTH };
	SDL_Rect rectc = { 0,SCREEN_HEIGHT - 5,SCREEN_WIDTH,5 };
	SDL_Rect rectd = { 0,0,SCREEN_WIDTH,5 };
	TextureManager::draw(floor, src, recta);
	TextureManager::draw(floor, src, rectb);
	TextureManager::draw(floor, src, rectc);
	TextureManager::draw(floor, src, rectd);
}

void Map::add_collision(int row, int column) { // adds a terrain/wall block to collision array
	for (int i = 0; i < SQUARE_SIZE; i++) {
		for (int j = 0; j < SQUARE_SIZE; j++) {
			collision_cords[(row * SQUARE_SIZE) + i][(column * SQUARE_SIZE) + j] = 1;
		}
	} 
}

