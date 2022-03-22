
#include "Game.h"
#include "TextureManager.h"
#include "GameObject.h"
#include "Map.h"
#include "Player.h"
#include "DeathScreen.h"
#include "MenuScreen.h"


//all the external classes objects
GameObject** star_objects;
GameObject** fairy_objects;
GameObject* dolphin_objects[DOLPHINS_NUMBER] = { NULL };
Player* player;
Map* map;
DeathScreen* d_screen;
MenuScreen* m_screen;
SDL_Rect Player_rect;


SDL_Renderer* Game::renderer = nullptr;
SDL_Rect Game::camera;

void Game::DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color) {
	int bpp = surface->format->BytesPerPixel;
	Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;
	*(Uint32*)p = color;
};


void Game::DrawLine(SDL_Surface* screen, int x, int y, int l, int dx, int dy, Uint32 color) {
	for (int i = 0; i < l; i++) {
		DrawPixel(screen, x, y, color);
		x += dx;
		y += dy;
	};
};



void Game::DrawString(SDL_Surface* screen, int x, int y, const char* text,
	SDL_Surface* charset) {
	int px, py, c;
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = 8;
	d.h = 8;
	while (*text) {
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_BlitSurface(charset, &s, screen, &d);
		x += 8;
		text++;
	};
};




void Game::DrawRectangle(SDL_Surface* screen, int x, int y, int l, int k,
	Uint32 outlineColor, Uint32 fillColor) {
	int i;
	DrawLine(screen, x, y, k, 0, 1, outlineColor);
	DrawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
	DrawLine(screen, x, y, l, 1, 0, outlineColor);
	DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);
	for (i = y + 1; i < y + k - 1; i++)
		DrawLine(screen, x + 1, i, l - 2, 1, 0, fillColor);
};






Game::Game() {
	collision_cords = NULL;
	level_height = level_width = 0;
	lives = LIVES;
	points = 0;
	overall_points = 0;
	display = GAME;
}

void Game::crash() { //activated when unicorn crashes into some obstacle
	lives--;
	if (lives > 0) {
		display = DEATH_SCREEN; //switch display based on lives count
		d_screen = new DeathScreen(points,screen,charset);
		overall_points += points;
		load_new_game(); //restart game 
	}
	else if (lives <= 0) {
		overall_points += points;
		display = MAIN_MENU;
		m_screen = new MenuScreen(overall_points, screen, charset);
		m_screen->load(overall_points);
	}
}


void Game::load_new_game() { //restart game 
	player = nullptr;
	player = new Player("./uni.bmp", collision_cords, level_height, level_width);
	shuffle();
	points = 0;
	fairies_count = 0;
	stars_count = 0;
}


void Game::init(const char* title, int width, int height, bool fullscreen) {


	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) {
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		}
		is_running = true;
	}
	else {
		printf("error: %s\n", SDL_GetError());
	}

	screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
		0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	charset = SDL_LoadBMP("./cs8x8.bmp");

	map = new Map();

	collision_cords = map->get_collision_cords();
	level_height = map->get_level_height();
	level_width = map->get_level_width();

	star = map->get_stars_arr();
	fairy = map->get_fairies_arr();

	stars = map->get_stars_number();
	fairies = map->get_fairies_number();

	hp = TextureManager::load_texture("./hearth.bmp", true);

	player = new Player("./uni.bmp", collision_cords, level_height, level_width);

	fairy_objects = (GameObject**)malloc(sizeof(GameObject*) * fairies);
	if (fairy_objects == NULL) {
		printf("error allocating memory");
	}
	star_objects = (GameObject**)malloc(sizeof(GameObject*) * stars);
	if (star_objects == NULL) {
		printf("error allocating memory");
	}

	m_screen = new MenuScreen(overall_points, screen, charset);
	
	

	
	


	shuffle(); //shuffle objects positions 
}

void Game::handle_events() {

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			is_running = false;
			break;

		default: break;
		}

		if (event.key.keysym.sym == SDLK_ESCAPE) {
			is_running = false;
			break;
		}
		else  if (event.key.keysym.sym == SDLK_n) {
			load_new_game(); //restart game
			points = 0;
			overall_points = 0;
			lives = LIVES;
		}

		switch (display) {
		case GAME:
			if (player->freeze == false) {
				player->handleEvent(event);
			}
			break;
		case DEATH_SCREEN:
			d_screen->handleEvent(event);
			break;
		case MAIN_MENU:
			m_screen->handleEvent(event);
			break;

		}
	}
}

void Game::update() {
	

	char text[128];
	int czarny = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
	int zielony = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
	int czerwony = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
	int niebieski = SDL_MapRGB(screen->format, 0x11, 0x11, 0xCC);

	reshuffle();
	destroy_popups();
	explode_stars();
	
	switch (display) {
	case GAME:


		points++;
		update_dolphins();
		player->move(time_n);
		update_camera();
		check_did_crash();
		draw_info(czerwony, niebieski, text);

		Player_rect = { player->get_pos_x(), player->get_pos_y(), player->model_height, player->model_width };

		check_stars_collision();
		check_fairy_collisions();


		break;
	case DEATH_SCREEN:
		if (d_screen->get_next_display() != DEATH_SCREEN) {
			handle_death_screen_continuation();
		}
		break;
	case MAIN_MENU:
		m_screen->load(overall_points);
		handle_menu_continuation();
		break;

	}
}
//---------------------------------------------------------------------UPDATE--------------------------------------------------------------------------------------

void Game::shuffle() { //allocate all the stars and fairies
	for (int i = 0; i < stars; i++) {
		star_objects[i] = new GameObject("./star.bmp", star[i], "star");
	}

	for (int i = 0; i < fairies; i++) {
		fairy_objects[i] = new GameObject("./fairy.bmp", fairy[i], "fairy");
	}
	reload(time(NULL)); //and randomize which ones will be visible
}


void Game::reload(int x) { //randomize which fairies and stars will appear
	srand(time(NULL) + x);
	for (int i = 0; i < stars; i++) {
		if (rand() % 2 > 0) { //chance should be aprox 50%
			star_objects[i] = NULL;
		}
	}

	for (int i = 0; i < fairies; i++) {
		if (rand() % 2 > 0) {
			fairy_objects[i] = NULL;
		}
	}
}



void Game::handle_death_screen_continuation() 
{
	display = d_screen->get_next_display();
	SDL_FreeSurface(screen); //free screen surface so previous screen disappears
	screen = nullptr;
	screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, //create a new one 
		0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	delete(d_screen);
	d_screen = NULL;

	if (display == MAIN_MENU) { // if player chooses to go to menu create and load one 
		m_screen = new MenuScreen(overall_points, screen, charset);
		m_screen->load(overall_points);
	}
}

void Game::handle_menu_continuation()
{
	if (m_screen->cont == true) {
		display = GAME;
		m_screen->cont = false;
		SDL_FreeSurface(screen); //free screen surface so previous screen disappears
		screen = nullptr;
		screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, //create a new one 
			0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
		delete(m_screen); //delete menu
		m_screen = NULL; 
		load_new_game(); //restart game
		points = 0;
		overall_points = 0;
		lives = LIVES;

	}
}

void Game::check_fairy_collisions()
{
	for (int i = 0; i < fairies; i++) { //fairy collision

		if (fairy_objects[i] != NULL) {
			srand(time(NULL)); //generate random number to pass it to fairy update function 
			fairy_objects[i]->Update(i + (rand() % 2) + 1);
		}
		if (fairy_objects[i] != NULL && SDL_HasIntersection(&Player_rect, &fairy[i]) && fairy_objects[i]->is_deleted() == false) { 
			fairy_objects[i] = NULL; //make fairy dissapear
			points += (fairies_count + 1) * 10; 
			SDL_Surface* tmp_srfc = SDL_CreateRGBSurface(0, 80, 20, 32, //temporary surface to draw popup to it 
				0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);;
			char p[8] = "";
			sprintf(p, "+ %d", (fairies_count + 1) * 10);
			DrawString(tmp_srfc, 0, 0, p, charset);
			points_popup = SDL_CreateTextureFromSurface(renderer, tmp_srfc);
			SDL_FreeSurface(tmp_srfc); //get rid of temp surface 
			destroy_timer = time_n;
			fairies_count++; // count this one as collected

		}
		if (fairy_objects[i] != NULL && fairy_objects[i]->is_deleted() == false && player->get_pos_x() > fairy_objects[i]->get_ini_x() + fairy_objects[i]->get_w()) { //if player passed fairy reset count 
			fairies_count = 0;
		}
	}
}

void Game::check_stars_collision()
{
	for (int i = 0; i < stars; i++) { //star collision

		if (star_objects[i] != NULL && SDL_HasIntersection(&Player_rect, &star[i]) && star_objects[i]->is_deleted() == false) {


			if (player->freeze != true) { //if player wasnt dashing
				player->set_posx(player->get_pos_x() - player->get_vx()); //stop player movement
				player->set_posy(player->get_pos_y() - player->get_vy()); 
				player->did_crash = true;  //crash
				crash_delay = time_n;
				star_objects[i]->delete_object();
				player->explode = true;
			}
			else {
				SDL_Surface* tmp_srfc = SDL_CreateRGBSurface(0, 80, 20, 32, //create surface to render a popup
					0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);;
				star_objects[i]->delete_object();
				star_objects[i]->timer = time_n;
				points += (stars_count + 1) * 100;
				char p[8] = "";
				sprintf(p, "+ %d", (stars_count + 1) * 100);
				DrawString(tmp_srfc, 0, 0, p, charset);
				points_popup = SDL_CreateTextureFromSurface(renderer, tmp_srfc);
				SDL_FreeSurface(tmp_srfc);
				destroy_timer = time_n;
				stars_count++; //increase count if player did collect this star

			}

		}
		//count =0 if player didnt collect the star and passed it 
		if (star_objects[i] != NULL && star_objects[i]->is_deleted() == false && player->get_pos_x() > star_objects[i]->get_ini_x() + star_objects[i]->get_w()) {
			stars_count = 0;
		}
	}
}

void Game::draw_info(int czerwony, int niebieski, char  text[128]) //draw essential info to screen
{
	DrawRectangle(screen, 4, 4, SCREEN_WIDTH - 8, 20, czerwony, niebieski);
	sprintf(text, "vx: %d vy: %d x: %d y: %d time: %5.2f s LIVES: %d POINTS: %d", player->get_vx(), player->get_vy(), player->get_pos_x(), player->get_pos_y(), time_n / 1000.0, lives, points);
	DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 10, text, charset);
	if (map->did_error() == true) {
		sprintf(text, "THERE WAS AN ERROR WHILE LOADING MAP");
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 10, text, charset);
	}

	info_window = SDL_CreateTextureFromSurface(renderer, screen);
}

void Game::check_did_crash() // check if collision with terrain/wall or star occured
{
	if (player->did_crash == true) {
		if (crash_delay == 0) {
			crash_delay = time_n; //set delay, so crash animation has time to be played
			player->freeze = true; //freeze, so unicorn cant move after crashing
			player->explode = true;
		}
		else if (crash_delay > 0) {
			if (time_n - crash_delay > 100) { //if time of unicorn destruction has come ...
				crash(); //destroy unicorn and reset crash delay
				player->did_crash = false;
				crash_delay = 0;
			}
		}
		else {
			crash();
			player->did_crash = false;
		}
	}
}

void Game::update_camera()// update camera so it tracks player
{
	camera.x = (player->get_pos_x() + Player::model_width / 2) - SCREEN_WIDTH / 2;
	camera.y = (player->get_pos_y() + Player::model_height / 2) - SCREEN_HEIGHT / 2;

	if (camera.x < 0)
	{
		camera.x = 0;
	}
	if (camera.y < 0)
	{
		camera.y = 0;
	}
	if (camera.x > level_width - SCREEN_WIDTH)
	{
		camera.x = level_width - SCREEN_WIDTH;
	}
	if (camera.y > level_height - SCREEN_HEIGHT)
	{
		camera.y = level_height - SCREEN_HEIGHT;
	}
}

void Game::update_dolphins()
{
	for (int i = 0; ((i < points / DOLPHIN_POINTS) && i < DOLPHINS_NUMBER); i++) {
		if (dolphin_objects[i] != NULL) { //if dolphin exists update 
			dolphin_objects[i]->Update(0);
		}
		else if (dolphin_objects[i] == NULL) { // if doesnt, create one 
			SDL_Rect doplhin_dest = { 0,SCREEN_HEIGHT - 40,60,30 };
			dolphin_objects[i] = new GameObject("./dolphin.bmp", doplhin_dest, "dolphin");
		}
	}
}

void Game::explode_stars() // delete star after its explosion
{
	for (int i = 0; i < stars; i++) {
		if (star_objects[i] != NULL && star_objects[i]->is_deleted() == true && time_n - star_objects[i]->timer > STAR_EXPLOSION_TIME) {
			star_objects[i] = NULL;
		}
	}
}

void Game::destroy_popups() //delete popups after POPUP_TIME
{
	if (destroy_timer > 0 && time_n - destroy_timer > POPUP_TIME) {
		SDL_DestroyTexture(points_popup);
		destroy_timer = 0;
	}
}



void Game::reshuffle() // reshuffle everytime player loops around map
{
	if (player->loop == true) {
		shuffle();
		player->loop = false;
	}
}



void Game::render() {
	SDL_RenderClear(renderer);
	switch (display) {
	case GAME:
		map->draw_map(camera.x, camera.y);
		render_stars();
		render_fairies();
		player->render(camera.x, camera.y);
		render_dolphins();
		render_popups();
		render_hp_bar();
		SDL_RenderCopy(renderer, info_window, NULL, NULL);
		SDL_DestroyTexture(info_window);
		break;
	case DEATH_SCREEN:
		d_screen->render();
		break;

	case MAIN_MENU:
		m_screen->render();
		break;
	}
	SDL_RenderPresent(renderer);
}

//-----------------------------------------------------------------------------------RENDER-------------------------------------------------------------------------------------
void Game::render_hp_bar()
{
	for (int i = 0; i < lives; i++) {
		TextureManager::draw(hp, { 0,0,135,135 }, { 10 + i * 20, 40 , 20, 20 });
	}
}
void Game::render_popups()
{
	if (destroy_timer > 0 && time_n - destroy_timer < POPUP_TIME) {
		TextureManager::draw(points_popup, { 0,0,40,8 }, { (SCREEN_WIDTH / 2) - 20,120,48,16 });
	}
}
void Game::render_dolphins()
{
	for (int i = 0; ((i < points / 500) && i<DOLPHINS_NUMBER); i++) {
		if (dolphin_objects[i] != NULL)
			dolphin_objects[i]->Render(camera.x, camera.y, player->get_pos_x() + i * 30, level_height - DOLPHIN_Y - 5 * i, "dolphin");
	}
}
void Game::render_fairies()
{
	for (int i = 0; i < fairies; i++) {
		if (fairy_objects[i] != NULL)
			fairy_objects[i]->Render(camera.x, camera.y, fairy_objects[i]->get_ini_x(), fairy_objects[i]->get_ini_y(), "fairy");
	}
}
void Game::render_stars()
{
	for (int i = 0; i < stars; i++) {
		if (star_objects[i] != NULL)
			star_objects[i]->Render(camera.x, camera.y, star_objects[i]->get_ini_x(), star_objects[i]->get_ini_y(), "star");
	}
}








void Game::clear() {
	for (int i = 0; i < stars; i++) {

		free(star_objects[i]);
	}
	for (int i = 0; i < fairies; i++) {
		free(fairy_objects[i]);
	}
	for (int i = 0; i < DOLPHINS_NUMBER; i++) {
		if (dolphin_objects[i] != NULL)
			free(dolphin_objects[i]);
	}
	SDL_DestroyTexture(hp);
	SDL_FreeSurface(charset);
	SDL_FreeSurface(screen);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}
