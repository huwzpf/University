#pragma once
#include "Game.h"




class Player
{

public:

	Player(const char* texture, int** cc, int height, int width);
	~Player(); 


	static const int model_width = 80;
	static const int model_height = 40;
	static const int model_velocity =20;




	void handleEvent(SDL_Event& e);

	void move(int time);

	void cast_angled_rays_left_bottom(int ray_casting_dis);

	void cast_angled_rays_right_top(int ray_casting_dis);

	void cast_angled_rays_right_bottom(int ray_casting_dis);

	void cast_angled_rays_left_top(int ray_casting_dis);

	void cast_rays_forwards(int ray_casting_dis);

	void cast_rays_backwards(int ray_casting_dis);

	void cast_rays_downwards(int ray_casting_dis);

	void cast_rays_upwards(int ray_casting_dis);

	void handle_collision_with_borders(int ray_casting_dis);

	void handle_move_special_conditions(int time_n);

	void render(int camx, int camy);

	void animate(SDL_Rect& src);

	void choose_texture();



	int get_pos_x() {
		return posx;
	}
	int get_pos_y() {
		return posy;
	}

	int get_vx() {
		return velx;
	}
	int get_vy() {
		return vely;
	}

	void set_posx(int n){
		posx = n;
	}

	void set_posy(int n){
		posy = n;
	}

	//bools specyfying unicorn's current state
	bool loop = false;
	bool controls = false;
	bool freeze = false;
	bool did_crash=false;
	bool explode = false;

private:
	bool dash=false;

	int posx, posy;
	int moves_count;
	int** collision_cords;
	int level_width, level_height;
	int velx, vely;
	int jumps_count = 0;
	int dash_start = -1;
	int animation_frames = 0;

	SDL_Rect src_rect, dest_rect;

	SDL_Texture* obj_texture;
	SDL_Texture* jump_sprites;
	SDL_Texture* uni_explosion;
	SDL_Texture* run_sprites;
	SDL_Texture* fall_sprites;

	
};

