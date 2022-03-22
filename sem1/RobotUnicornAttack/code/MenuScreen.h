#pragma once
#include "Game.h"
class MenuScreen
{
public:
	MenuScreen(int points, SDL_Surface* screen, SDL_Surface* charset);

	void handleEvent(SDL_Event& e);
	void handle_highscores_screen(SDL_Event& e);
	void handle_saving(SDL_Event& e);

	void render();

	void save();

	void load(int p);
	void handle_views(int czerwony, int niebieski, int zielony);
	void show_highscores();
	void display_sorted_array(int count, FILE* file, char  p_name[32], int& p_score, int* scores_arr, int& count_a);
	void sort_array(FILE* file, int& p_score, int* scores_arr, int& count_a, int count);
	void count_records(FILE* file, int& count);

	bool cont = false; // if its true display in game.cpp continues to game screen 

private:
	char text_menu[128]; //variable storing all the text on page
	bool showing_hs = false; //flags indicating what is currently displayed on screen 
	bool saving = false; 
	int points;
	char input_text[32];
	int current_site, sites;
	const int site_capacity = 5;// how many elements one site of highscores can fit

	SDL_Texture* txt_to_display;
	SDL_Surface* charset;
	SDL_Surface* screen;

};

