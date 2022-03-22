#include "MenuScreen.h"
#include "TextureManager.h"



MenuScreen::MenuScreen(int p, SDL_Surface* scr, SDL_Surface* chset) {

	screen = scr;
	charset = chset;
	points = p;
	*input_text = NULL;
	showing_hs = false;
	current_site = 0;
}




void MenuScreen::handleEvent(SDL_Event& e) {



	if (e.type == SDL_KEYDOWN)
	{
		if (e.key.keysym.sym == SDLK_c) { // leave menu
			cont = true;
			SDL_DestroyTexture(txt_to_display);
		}


		handle_saving(e);
		handle_highscores_screen(e);
	}
	else if (e.type == SDL_TEXTINPUT && strlen(input_text)<32) { //handle text input
		
		char* c = e.text.text;
		strcat(input_text, c);
		SDL_StopTextInput();
	}
	
}



void MenuScreen::load(int p) {
	//colors used in displaying image
	int czarny = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
	int zielony = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
	int czerwony = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
	int niebieski = SDL_MapRGB(screen->format, 0x11, 0x11, 0xCC);



	points = p;


	Game::DrawRectangle(screen, 4, 4, SCREEN_WIDTH - 20, SCREEN_HEIGHT - 20, czerwony, niebieski);
	Game::DrawRectangle(screen, 40, 20, SCREEN_WIDTH - 80, SCREEN_HEIGHT - 100, zielony, czarny);

	sprintf(text_menu, "GAME OVER");
	Game::DrawString(screen, screen->w / 2 - strlen(text_menu) * 8 / 2, 30, text_menu, charset);
	sprintf(text_menu, "PRESS C TO GET BACK TO GAME");
	Game::DrawString(screen, screen->w / 2 - strlen(text_menu) * 8 / 2, 50, text_menu, charset);
	sprintf(text_menu, "YOUR POINTS: %d", points);
	Game::DrawString(screen, screen->w / 2 - strlen(text_menu) * 8 / 2, 70, text_menu, charset);

	handle_views(czerwony, niebieski, zielony);

	txt_to_display = SDL_CreateTextureFromSurface(Game::renderer, screen);
}



void MenuScreen::save() {
	FILE* file;
	file = fopen("highscores.txt", "a+");
	fprintf(file, "Player: %s Score: %d\n", input_text, points);
	fclose(file);
}

void MenuScreen::show_highscores() {
	int count = 0;
	int count_a = 0;
	int p_score = 0;
	char p_name[32];
	FILE* file;
	file = fopen("./highscores.txt", "r");

	count_records(file, count);

	int* scores_arr = (int*)malloc(count * sizeof(int)); //malloc memory for array after counting records
	if (scores_arr == NULL) {
		printf("error allocating memory");
	}
	sort_array(file, p_score, scores_arr, count_a, count);

	display_sorted_array(count, file, p_name, p_score, scores_arr, count_a);

	fclose(file);
}




void MenuScreen::render() { // render to the whole screen 
	SDL_Rect rect;
	rect.x = rect.y = 1;
	rect.h = SCREEN_HEIGHT;
	rect.w = SCREEN_WIDTH;
	TextureManager::draw(txt_to_display, rect, rect);
	SDL_DestroyTexture(txt_to_display); // get rid of already used texture, so it can update if display changes
}



//------------------------------------------------------------INPUT HANDLING----------------------------------------------------------------------------
void MenuScreen::handle_highscores_screen(SDL_Event& e)
{

	if (showing_hs == true) { //on arrow presses switch between pages
		if (e.key.keysym.sym == SDLK_RIGHT) {
			if (current_site < sites - 1) {
				current_site++;
			}
			else {
				current_site = 0;
			}
		}
		if (e.key.keysym.sym == SDLK_LEFT) {
			if (current_site > 0) {
				current_site--;
			}
			else {
				current_site = sites - 1;
			}
		}

	}


	else if (showing_hs == false) { //applies to situation when needs to choose if he wants to save his nick (enter) or no (n) 
		if (e.key.keysym.sym == SDLK_RETURN) {
			saving = true;
			SDL_StartTextInput();
		}
		if (e.key.keysym.sym == SDLK_n) {
			showing_hs = true;
		}
	}
}




void MenuScreen::handle_saving(SDL_Event& e)
{
	if (saving == true) { 

		if (e.key.keysym.sym == SDLK_BACKSPACE && strlen(input_text) > 0) //handle backspace press
		{
			input_text[strlen(input_text) - 1] = '\0';
		}
		if (e.key.keysym.sym == SDLK_RETURN) { // on pressing enter stop taking text input and save 
			saving = false;
			SDL_StopTextInput();
			save();
			showing_hs = true;
		}
	}
}




//--------------------------------------------------------------------------------------OUTPUT------------------------------------------------------------------------------------------------------------
void MenuScreen::handle_views(int czerwony, int niebieski, int zielony)
{
	if (showing_hs == false) {
		sprintf(text_menu, "DO YOU WANT TO SAVE YOUR SCORE ?  ENTER - YES N - NO");
		Game::DrawString(screen, screen->w / 2 - strlen(text_menu) * 8 / 2, 120, text_menu, charset);
	}
	else if (showing_hs == true) {
		sprintf(text_menu, "HIGHSCORES");
		Game::DrawString(screen, screen->w / 2 - strlen(text_menu) * 8 / 2, 100, text_menu, charset);
		sprintf(text_menu, "page %d / %d", current_site + 1, sites);
		Game::DrawString(screen, screen->w / 2 - strlen(text_menu) * 8 / 2, 120, text_menu, charset);
		show_highscores();
	}

	if (saving == true) {
		sprintf(text_menu, "ENTER YOUR NICKNAME :");
		Game::DrawRectangle(screen, 50, 160, SCREEN_WIDTH - 100, 24, czerwony, niebieski);

		Game::DrawRectangle(screen, 260, 162, 300, 18, czerwony, zielony);
		Game::DrawString(screen, 270, 164, input_text, charset);
		Game::DrawString(screen, 60, 164, text_menu, charset);
		SDL_StartTextInput();
	}
}

int compare(const void* a, const void* b) // neded for qsort
{
	return (*(int*)b - *(int*)a);
}



//------------------------------------------------------------------------SHOW HIGHSCORES---------------------------------------------------------------------------------------------------------------------------		

void MenuScreen::display_sorted_array(int count, FILE* file, char  p_name[32], int& p_score, int* scores_arr, int& count_a)
{
	for (int i = 0; i < count && i < site_capacity; i++) {
		fseek(file, 0, SEEK_SET);
		while (fscanf(file, "Player: %s Score: %d\n", p_name, &p_score) != EOF) { //read all the records from file 

			if (p_score == scores_arr[i + (site_capacity * current_site)]) { //if currently red record matches currently displayed position in array print it 
				sprintf(text_menu, "Player: %s Score: %d", p_name, p_score);
				Game::DrawString(screen, screen->w / 2 - strlen(text_menu) * 8 / 2, 140 + count_a * 20, text_menu, charset);
				scores_arr[i + (site_capacity * current_site)] = -1;
				continue; //after displaying one record go to another
			}
		}
		count_a++;
	}
}

void MenuScreen::sort_array(FILE* file, int& p_score, int* scores_arr, int& count_a, int count)
{

	fseek(file, 0, SEEK_SET);
	while (fscanf(file, "Player: %*s Score: %d\n", &p_score) != EOF) {
		scores_arr[count_a] = p_score; //save all the records in array
		count_a++;
	}
	qsort(scores_arr, count, sizeof(int), compare); //sort array
	count_a = 0;
}

void MenuScreen::count_records(FILE* file, int& count)
{
	fseek(file, 0, SEEK_SET);
	while (fscanf(file, "Player: %*s Score: %*d\n") != EOF) {
		count++;
	}

	sites = count / site_capacity;
	if (sites * site_capacity < count)
		sites++;
}


