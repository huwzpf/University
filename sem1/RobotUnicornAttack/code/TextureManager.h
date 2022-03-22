#pragma once
#include "Game.h"
class TextureManager {
public:
	static SDL_Texture* load_texture(const char* filename, bool key);
	static void draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest);
};