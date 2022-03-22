#include "Game.h"
#include "TextureManager.h"
		Game* game = nullptr;

		int main(int argc, char* argv[]) {


			const int FPS = 30;
			const int frame_delay = 1000 / FPS;
			Uint32 frame_start, time=0; 
			int frame_time;

				game = new Game();
				game->init("title", SCREEN_WIDTH, SCREEN_HEIGHT, false);
				while (game->running()) {

					frame_start = SDL_GetTicks(); //start timer

					game->handle_events();
					game->update();
					game->render();

					frame_time = SDL_GetTicks() - frame_start; 

					if (frame_delay > frame_time) {
						SDL_Delay(frame_delay - frame_time); //if game is running too fast delay ext frame 
					}

					time += SDL_GetTicks() - frame_start; //calculate time 
					game->set_time(time);
				}
				game->clear();

			return 0;
		}