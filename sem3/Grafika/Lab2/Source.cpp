
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>

// Do��cz definicje biblioteki Allegro
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

const float FPS = 60;		//obraz b�dzie aktualizowany co 1/FPS sekundy
const int SCREEN_W = 640;	//szeroko�� okna
const int SCREEN_H = 480;	//wysoko�� okna

ALLEGRO_COLOR yellow = al_map_rgb(255, 255, 0);
ALLEGRO_COLOR white = al_map_rgb(255, 255, 255);
ALLEGRO_COLOR blue = al_map_rgb(0, 0, 255);
ALLEGRO_COLOR black = al_map_rgb(0, 0, 0);

void zadanie(int n, float x, float y, float r, double* fi);

// Funkcja g��wna
int main()
{
	ALLEGRO_DISPLAY* display = NULL;			//okno
	ALLEGRO_EVENT_QUEUE* event_queue = NULL;	//kolejka zdarzen
	ALLEGRO_TIMER* timer = NULL;				//timer, od ktorego b�dziemy odbierac zdarzenia (potrzebny do animacji)
	bool redraw = true;

	if (!al_init()) {							//inicjalizacja biblioteki Allegro
		fprintf(stderr, "Nie zainicjalizowano allegro!\n");
		return -1;
	}

	display = al_create_display(SCREEN_W, SCREEN_H);	//utworznie okna
	timer = al_create_timer(1.0 / FPS);					//utworzenie timera
	al_install_keyboard();								//inicjalizacja obs�ugi klawiatury
	event_queue = al_create_event_queue();				//utworzenie kolejki zdarze�

	al_init_primitives_addon();							//inicjalizacja obs�ugi prostych element�w (punkty, linie, prostok�ty, elipsy itd.)

	//Rejestracja �r�de� zdarze� (okno, timer, klawiatura ...)
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	//Uruchamiamy timer, kt�ry b�dzie z zadan� cz�stotliwo�ci� wysy�a� zdarzenia
	al_start_timer(timer);

	//Zmienne na potrzeby obracania figury
	double fi = 0.0;
	//


	//P�tla g��wna programu - obs�uga zdarze�.
	//Dzia�amy, dop�ki u�ytkownik nie wci�nie Esc.
	while (true)
	{
		ALLEGRO_EVENT event;
		al_wait_for_event(event_queue, &event);

		if (event.type == ALLEGRO_EVENT_TIMER) {	//zdarzenie timera -> od�wie�enie obrazu 
			redraw = true;
		}
		else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {	//zdarzenie klawiatury -> je�li Esc to ko�czymy
			if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
				break;
		}
		else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) { //zdarzenie zamkni�cia okna
			break;
		}

		if (redraw && al_is_event_queue_empty(event_queue))
		{
			redraw = false;

			al_clear_to_color(black); //czyszczenie okna na zadany kolor

			zadanie(6, 320, 180, 20, &fi);

			//Wy�wietl w oknie to, co narysowano w buforze
			al_flip_display();
		}
	}

	al_destroy_display(display);
	al_destroy_timer(timer);
	al_destroy_event_queue(event_queue);
	return 0;
}

void zadanie(int n, float x, float y, float r, double* fi) {
	// Zmienne potrzebne do obrotu
	//double sinfi_x = sin(1.5 * *fi);
	//double cosfi_x = cos(1.5 * *fi);
	double sinfi= sin(*fi);
	double cosfi = cos(*fi);
	float R = 3 * r;
	// Wyznaczanie wsp�rz�dnych ko�a
	float x_updated;
	float y_updated;
	x_updated = R * sinfi + x;
	y_updated = R * sinfi * cosfi  + y;
	// Rysowanie ko�a ma�ego
	al_draw_filled_circle(x_updated, y_updated, r, blue);
	
	*fi += 0.05;
}

