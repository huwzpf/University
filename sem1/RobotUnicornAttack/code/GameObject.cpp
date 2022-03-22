#include "GameObject.h"
#include "TextureManager.h"
#define DOLPHIN_DURATION 60

GameObject::GameObject(const char* texture,  SDL_Rect rect ,const char* type) { //rect is initial position and size of object, type specifies what actions should be performed with it during runtime 
	obj_texture = TextureManager::load_texture(texture,true);
	if (type == "star") {
		explosion_texture= TextureManager::load_texture("./star_explosion.bmp", true); //stars explode so it needs texture
	}
	obj_type = type;
	dest_rect = rect;
	initial_x = rect.x;
	initial_y = rect.y;
	dx = initial_x;
	dy = initial_y;

}

GameObject::~GameObject() {
	SDL_DestroyTexture(explosion_texture);
	SDL_DestroyTexture(obj_texture);
}

void GameObject::Update(int a) {
		moves_count++;

	if (obj_type == "fairy") {  //fairies move with sin function and are dependent from random a value
			
			if (((a % 2 > 0?-1:1)*sin(moves_count/4)) > 0 ) {
				initial_x += 2;
				initial_y-=2;
			}
			else {
				initial_x -= 2;
					initial_y+=2;
			}
				

			if (initial_x - dx > 30) { //fairies cant move more than 30px from inititial coordinates
				initial_x -= 2;
			}
			else if (initial_x - dx < -30) {
				initial_x += 2;
			}
			
			if (initial_y - dy > 30) {
				initial_y-=2;
			}
			else if (initial_y - dy < -30) {
				initial_y+=2;
			}
		
	}
}

void GameObject::Render(int x, int y, int i_x, int i_y, const char* type) {
	int y_offset = 0;
	int x_offset = 0;
	
	
	if (type == "dolphin") { //dolphins move with abs function
		x_offset = moves_count / 2;
		y_offset = 2*abs((moves_count % (2 * DOLPHIN_Y)) - DOLPHIN_Y)-DOLPHIN_Y/2;
		
	}

	dest_rect.x = i_x - x + x_offset; //if object isnt dolphin offset will be 0
	dest_rect.y = i_y - y + y_offset;



	SDL_Rect src_rect; // assign values to source rectangle 
	src_rect.x = src_rect.y=0;
	src_rect.h = src_rect.w = 130; //source rectangle is 130x130 bmp file
	animate_star_explosion(type, src_rect);

	TextureManager::draw(obj_texture, src_rect, dest_rect);

}

void GameObject::animate_star_explosion(const char* type, SDL_Rect& src_rect)
{
	if (type == "star" && del == true) {
		src_rect.h = src_rect.w = 64; //animatioin source rectangle is 64x64 bmp file 
		int x_arr[9];
		int y_arr[9];
		for (int i = 0; i < 9; i++) { // calculate positions of each sprite
			x_arr[i] = 64 * (i % 3);
			y_arr[i] = 64 * (i % 3);
		}

		src_rect.x = x_arr[animation_frames];
		src_rect.y = y_arr[animation_frames];

		obj_texture = explosion_texture;
		if (moves_count % 4 == 0)animation_frames++; //every 4th frame update animation 
		if (animation_frames == 6) { //cycle animation_frames between 0-6
			animation_frames = 0;
		}

	}
}


