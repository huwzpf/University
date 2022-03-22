#pragma once
#include "Game.h"
class DeathScreen
{
public:
	DeathScreen(int points, SDL_Surface* screen, SDL_Surface* charset);
	~DeathScreen();
	void handleEvent(SDL_Event& e);
	void render();
	current_display get_next_display() {
		return next_display;
	}
private:
	current_display next_display=DEATH_SCREEN;
	SDL_Texture* txt_to_display;


	char text1[128];
};

