
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>

// Do³¹cz definicje biblioteki Allegro
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

const float FPS = 60;		//obraz bêdzie aktualizowany co 1/FPS sekundy
const int SCREEN_W = 640;	//szerokoœæ okna
const int SCREEN_H = 480;	//wysokoœæ okna

ALLEGRO_COLOR yellow = al_map_rgb(255, 255, 0);
ALLEGRO_COLOR white = al_map_rgb(255, 255, 255);
ALLEGRO_COLOR blue = al_map_rgb(0, 0, 255);
ALLEGRO_COLOR black = al_map_rgb(0, 0, 0);

void zadanie(int n, float x, float y, float r, double* fi);

// Funkcja g³ówna
int main()
{
	ALLEGRO_DISPLAY* display = NULL;			//okno
	ALLEGRO_EVENT_QUEUE* event_queue = NULL;	//kolejka zdarzen
	ALLEGRO_TIMER* timer = NULL;				//timer, od ktorego bêdziemy odbierac zdarzenia (potrzebny do animacji)
	bool redraw = true;

	if (!al_init()) {							//inicjalizacja biblioteki Allegro
		fprintf(stderr, "Nie zainicjalizowano allegro!\n");
		return -1;
	}

	display = al_create_display(SCREEN_W, SCREEN_H);	//utworznie okna
	timer = al_create_timer(1.0 / FPS);					//utworzenie timera
	al_install_keyboard();								//inicjalizacja obs³ugi klawiatury
	event_queue = al_create_event_queue();				//utworzenie kolejki zdarzeñ

	al_init_primitives_addon();							//inicjalizacja obs³ugi prostych elementów (punkty, linie, prostok¹ty, elipsy itd.)

	//Rejestracja ¿róde³ zdarzeñ (okno, timer, klawiatura ...)
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	//Uruchamiamy timer, który bêdzie z zadan¹ czêstotliwoœci¹ wysy³a³ zdarzenia
	al_start_timer(timer);

	//Zmienne na potrzeby obracania figury
	double fi = 0.0;
	//


	//Pêtla g³ówna programu - obs³uga zdarzeñ.
	//Dzia³amy, dopóki u¿ytkownik nie wciœnie Esc.
	while (true)
	{
		ALLEGRO_EVENT event;
		al_wait_for_event(event_queue, &event);

		if (event.type == ALLEGRO_EVENT_TIMER) {	//zdarzenie timera -> odœwie¿enie obrazu 
			redraw = true;
		}
		else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {	//zdarzenie klawiatury -> jeœli Esc to koñczymy
			if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
				break;
		}
		else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) { //zdarzenie zamkniêcia okna
			break;
		}

		if (redraw && al_is_event_queue_empty(event_queue))
		{
			redraw = false;

			al_clear_to_color(black); //czyszczenie okna na zadany kolor

			zadanie(6, 320, 180, 20, &fi);

			//Wyœwietl w oknie to, co narysowano w buforze
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
	// Wyznaczanie wspó³rzêdnych ko³a
	float x_updated;
	float y_updated;
	x_updated = R * sinfi + x;
	y_updated = R * sinfi * cosfi  + y;
	// Rysowanie ko³a ma³ego
	al_draw_filled_circle(x_updated, y_updated, r, blue);
	
	*fi += 0.05;
}

