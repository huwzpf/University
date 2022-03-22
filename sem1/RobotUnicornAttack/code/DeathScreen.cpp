#include "DeathScreen.h"
#include "TextureManager.h"

void DeathScreen::handleEvent(SDL_Event& e){

    if (e.type == SDL_KEYDOWN)
    {
        if (e.key.keysym.sym == SDLK_z) {
            next_display = GAME;
        }
        if (e.key.keysym.sym == SDLK_x) {
            next_display = MAIN_MENU;
        }
    }
}

DeathScreen::~DeathScreen() {
	SDL_DestroyTexture(txt_to_display);
}


DeathScreen::DeathScreen(int points, SDL_Surface* screen, SDL_Surface* charset) {
	
	//colors used in displaying
	 int czarny = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
	 int zielony = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
	int czerwony = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
	 int niebieski = SDL_MapRGB(screen->format, 0x11, 0x11, 0xCC);


	 txt_to_display = NULL;
	Game::DrawRectangle(screen, 4, 4, SCREEN_WIDTH -20, SCREEN_HEIGHT-20, czerwony, niebieski);
	Game::DrawRectangle(screen, 40, 20, SCREEN_WIDTH -80, SCREEN_HEIGHT-100, zielony, czarny);
	sprintf(text1, "YOUR POINTS: %d", points);
	Game::DrawString(screen, screen->w / 2 - strlen(text1) * 8 / 2, 30, text1, charset);
	sprintf(text1, "PRESS Z TO PLAY AGAIN, PRESS X TO QUIT");
	Game::DrawString(screen, screen->w / 2 - strlen(text1) * 8 / 2, 70, text1, charset);
	txt_to_display = SDL_CreateTextureFromSurface(Game::renderer, screen);
}

void DeathScreen::render() {
	SDL_Rect rect;
	rect.x = rect.y = 0;
	rect.h = SCREEN_HEIGHT;
	rect.w = SCREEN_WIDTH;
	TextureManager::draw(txt_to_display, rect, rect);
	
}