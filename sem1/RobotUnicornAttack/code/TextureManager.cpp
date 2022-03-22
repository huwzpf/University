#include "TextureManager.h"

SDL_Texture* TextureManager::load_texture(const char* filename, bool key) {
	SDL_Surface* temp_srfc = SDL_LoadBMP(filename);
	if (key == true) {
		SDL_SetColorKey(temp_srfc, SDL_TRUE, SDL_MapRGB(temp_srfc->format, 255, 255, 255));
	}
	SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, temp_srfc);
	SDL_FreeSurface(temp_srfc);
	return tex;
}

void TextureManager::draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest) {
	if(tex!=NULL)
	SDL_RenderCopy(Game::renderer, tex, &src, &dest);
}