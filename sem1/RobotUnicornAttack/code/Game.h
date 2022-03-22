#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<math.h>
#include<stdio.h>
#include<string.h>
#include<time.h>
#include<stdlib.h>
#define SCREEN_WIDTH 640	
#define SCREEN_HEIGHT 480

#define JUMP_SPEED 25
#define DASH_SPEED 10
#define DASH_TIME 500
#define MAX_FALLING_SPEED 15
#define FALLING_UPDATE_RATE 1
#define FALLING_SPEED_UPDATE_AMOUNT 1
#define MAX_FORWARD_SPEED 15
#define FORWARD_UPDATE_RATE 5
#define FORWARD_SPEED_UPDATE_AMOUNT 2

#define DOLPHINS_NUMBER 5
#define DOLPHIN_Y 40
#define DOLPHIN_POINTS 500

#define STAR_EXPLOSION_TIME 500
#define POPUP_TIME 500
#define LIVES 3






extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}


typedef enum current_display {
	MAIN_MENU,
	DEATH_SCREEN,
	GAME
};	


class Game {
public:


	Game();

	void init(const char* title, int width, int height, bool fullscreen);
	void handle_events();
	void update();
	void handle_death_screen_continuation();
	void handle_menu_continuation();
	void check_fairy_collisions();
	void check_stars_collision();
	void draw_info(int czerwony, int niebieski, char  text[128]);
	void check_did_crash();
	void update_camera();
	void update_dolphins();
	void explode_stars();
	void destroy_popups();
	void reshuffle();
	void render();
	void render_hp_bar();
	void render_popups();
	void render_dolphins();
	void render_fairies();
	void render_stars();
	void clear();
	bool running() {
		return is_running;
	}
	void crash();
	void reload(int x);
	static SDL_Renderer* renderer;
	static SDL_Rect camera;
	Uint32 get_time() {
		return time_n;
	 }

	void set_time(Uint32 time_r) {
		time_n = time_r;
	}
	int get_lives() {
		return lives;
	}
	void shuffle();

	static void DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color);

	static void DrawLine(SDL_Surface* screen, int x, int y, int l, int dx, int dy, Uint32 color);

	static void DrawString(SDL_Surface* screen, int x, int y, const char* text,
		SDL_Surface* charset);

	static void DrawRectangle(SDL_Surface* screen, int x, int y, int l, int k,
		Uint32 outlineColor, Uint32 fillColor);
	void load_new_game();

	
	


private:
	
	bool is_running = false;

	current_display display;


	SDL_Surface* screen;
	SDL_Surface* charset;
	SDL_Texture* points_popup;
	SDL_Rect* star;
	SDL_Rect* fairy;
	SDL_Texture* info_window;
	SDL_Texture* hp;
	SDL_Window* window;

	Uint32 time_n;

	int stars, fairies, dolphins;
	int lives;
	int** collision_cords;
	int level_width, level_height; 
	int fairies_count=0, stars_count=0;
	int overall_points=0, points=0;
	int destroy_timer; // timer counting when to get rid of object
	int crash_delay=0;

};









