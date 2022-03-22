#pragma once
#include "Game.h"
class GameObject
{
public:

	GameObject(const char* texture, SDL_Rect dest_rect ,const char* type);
	~GameObject();

	void Update(int a);
	void Render(int x, int y, int i_x, int i_y, const char* type);
	void animate_star_explosion(const char* type, SDL_Rect& src_rect);
	int get_ini_x() {
		return initial_x;
	}

	int get_ini_y() {
		return initial_y;
	}

	int get_w() {
		return dest_rect.w;
	}

	void delete_object() {
		del = true;
	}

	bool is_deleted() {
		return del;
	}


	int timer;
	

private:

	SDL_Texture* obj_texture;
	SDL_Texture* explosion_texture = NULL;

	const char* obj_type;
	int dx = 0, dy = 0;
	int moves_count = 0;

	bool is_destroyed; //bool indicating was object hit by player
	

	SDL_Rect  dest_rect;
	int initial_x, initial_y;
	bool del = false;
	char* type;
	int animation_frames = 0;
};


