#include<allegro5\allegro.h>
#include<allegro5\allegro_native_dialog.h>
#include<allegro5\allegro_font.h>
#include<allegro5\allegro_ttf.h>
#include<allegro5\allegro_image.h>
#include<allegro5\allegro_primitives.h>
#include <allegro5\allegro_audio.h>
#include <allegro5\allegro_acodec.h>
#include <allegro5\monitor.h>
#include <ctime>

#define FPS 60
#define PREDKOSC_KUL 6
#define OPOZNIENIE_STRZELANIA 0.6f

enum KEYS { UP, DOWN, LEFT, RIGHT, SPACE };
enum KEYS1 { W, S, A, D };

int shot = 0;
const int MAX_BULLETS = 100;

typedef struct obiekt
{
	int x, y, kierunek, alive;
} obiekt;

obiekt bullets[MAX_BULLETS];
int add_bullet(int x, int y, int kierunek)
{

	int i;

	for (i = 0; i < MAX_BULLETS; i++) {
		if (bullets[i].alive == 0) {
			bullets[i].x = x;
			bullets[i].y = y;
			bullets[i].kierunek = kierunek;
			return i; 
		}
	}

	return(0); 
}

// pocisk
////////////////////////

int width;
int height;

////////////inicjalizacja zmiennych//////////////
bool wyszedl = false;
bool powrocil = true;
bool zablokowane[] = { false };
bool keys[5] = { false, false, false, false, false };
bool keys1[4] = { false, false, false, false };
int menu[] = { 0, 0, 0 };
int zliczaj_enter = 0;
int zapamietaj, jeszcze_jeden_enter;
bool muzyka_wlaczona = true;
bool ostatni_wybor_muzyka = true;
bool opuscil_menu = false;
bool in_game = false;
bool wybor_ilosci_graczy[2] = { false, false };
int zliczaj_enter_wybor = 0;
int pos_x = 0;
int pos_y = 0;
int kierunek = -1;
// wejsciowe
ALLEGRO_BITMAP *BMP_WEJSCIOWE = NULL;
ALLEGRO_BITMAP *BMP_NOWA_GRA = NULL;
ALLEGRO_BITMAP *BMP_CREDITS = NULL;
ALLEGRO_BITMAP *BMP_ZAKONCZ = NULL;
ALLEGRO_BITMAP *BMP_OPCJE = NULL;
ALLEGRO_BITMAP *BMP_REKORDY = NULL;
ALLEGRO_BITMAP *ikona = NULL;
ALLEGRO_BITMAP *BMP_JEDEN_GRACZ = NULL;
ALLEGRO_BITMAP *BMP_DWOCH_GRACZY = NULL;
ALLEGRO_BITMAP *BMP_MUZYKA = NULL;
ALLEGRO_BITMAP *BMP_MUZYKA_STAN = NULL;
//postac//
ALLEGRO_BITMAP *BMP_POSTAC_GORA = NULL;
ALLEGRO_BITMAP *BMP_POSTAC_DOL = NULL;
ALLEGRO_BITMAP *BMP_POSTAC_LEWO = NULL;
ALLEGRO_BITMAP *BMP_POSTAC_PRAWO = NULL;
ALLEGRO_BITMAP *BMP_POSTAC = NULL;
ALLEGRO_BITMAP *BMP_POCISK = NULL;
//////////
ALLEGRO_BITMAP *BMP_POZIOM = NULL;
ALLEGRO_BITMAP *BMP_SYMBOL = NULL;


// mjuzik //
ALLEGRO_SAMPLE *sample = NULL;
//

void rysuj_poziom(int ktory) {
	switch (ktory) {
	case 1: {
		BMP_WEJSCIOWE = al_load_bitmap("mapybmp/pierwsza.png");
		BMP_SYMBOL = al_load_bitmap("mapybmp/I.png");
		al_draw_bitmap(BMP_WEJSCIOWE, 0, 0, 0);
		al_draw_bitmap(BMP_SYMBOL, width / 2 + 10, height - 80, 0);
		break;
		}
	}
}
void pre_start_game() {
	ALLEGRO_EVENT_QUEUE *event_queue_stage = NULL;
	ALLEGRO_TIMER *timer_stage = NULL;
	int i, temp;
	pos_x = width / 2 - 50;
	pos_y = height-80;
	kierunek = 0;
	float tajmer = -1.0f;
	event_queue_stage = al_create_event_queue();
	timer_stage = al_create_timer(1.0 / FPS);
	al_start_timer(timer_stage);
	/////////////// eventy
	al_register_event_source(event_queue_stage, al_get_keyboard_event_source());
	al_register_event_source(event_queue_stage, al_get_timer_event_source(timer_stage));
	rysuj_poziom(1);
	for (i = 0; i < MAX_BULLETS; i++)
	{
		bullets[i].alive = 0;
	}
	while (opuscil_menu && in_game) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue_stage, &ev);
		if (tajmer > 0.0f) {
			tajmer = tajmer - (1.0 / FPS);
		}
		if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_UP:
				keys[UP] = true;
				kierunek = 0;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = true;
				kierunek = 1;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = true;
				kierunek = 2;
				break;

			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = true;
				kierunek = 3;
				break;
			case ALLEGRO_KEY_SPACE:
				keys[SPACE] = true;
				if (tajmer <= 0) {
					temp = add_bullet(pos_x, pos_y + 20, kierunek);
					shot = 1;
					bullets[temp].alive = 1;
					tajmer = OPOZNIENIE_STRZELANIA;
				}
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_UP:
				keys[UP] = false;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = false;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = false;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = false;
				break;
			case ALLEGRO_KEY_SPACE:
				keys[SPACE] = false;
				if (shot == 1) {
					shot = 0;
				}
				break;
			case ALLEGRO_KEY_ESCAPE:
				in_game = false;
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_TIMER) {
			al_draw_bitmap(BMP_WEJSCIOWE, 0, 0, 0);
			for (i = 0; i < MAX_BULLETS; i++) 
			{
				if (bullets[i].alive == 1)
				{
					al_draw_bitmap(BMP_POCISK, bullets[i].x, bullets[i].y, 0);
				}
			}
			for (i = 0; i < MAX_BULLETS; i++)
			{
				if (bullets[i].alive)
				{
					switch (bullets[i].kierunek) {
					case 0: bullets[i].y-=PREDKOSC_KUL; break;
					case 1: bullets[i].y+=PREDKOSC_KUL; break;
					case 2: bullets[i].x-=PREDKOSC_KUL; break;
					case 3: bullets[i].x+=PREDKOSC_KUL; break;
					}
				}
			}
			for (i = 0; i < MAX_BULLETS; i++)
			{
				if (bullets[i].alive)
				{
					if (bullets[i].x < 0 || bullets[i].y < 0 || bullets[i].x > width || bullets[i].y > height)
						bullets[i].alive = 0;
				}
			}
			al_draw_bitmap(BMP_SYMBOL, width / 2 + 10, height - 80, 0);
			pos_y -= keys[UP] * 5;
			pos_y += keys[DOWN] * 5;
			pos_x -= keys[LEFT] * 5;
			pos_x += keys[RIGHT] * 5;
			if (pos_y <= 0) pos_y = 0;
			if (pos_x <= 0) pos_x = 0;
			if (pos_y + al_get_bitmap_height(BMP_POSTAC_DOL) >= height) pos_y = height - al_get_bitmap_height(BMP_POSTAC_DOL);
			if (pos_x + al_get_bitmap_width(BMP_POSTAC_GORA) >= width) pos_x = width - al_get_bitmap_width(BMP_POSTAC_GORA);
			switch (kierunek) {
			case 0:al_draw_bitmap(BMP_POSTAC_GORA, pos_x, pos_y, 0); break;
			case 1:al_draw_bitmap(BMP_POSTAC_DOL, pos_x, pos_y, 0); break;
			case 2:al_draw_bitmap(BMP_POSTAC_LEWO, pos_x, pos_y, 0); break;
			case 3: al_draw_bitmap(BMP_POSTAC_PRAWO, pos_x, pos_y, 0); break;
			}
			al_flip_display();
		}

		}
	}


	int main(void) {
		ALLEGRO_DISPLAY_MODE   disp_data;
		ALLEGRO_MONITOR_INFO Monitor_info;
		ALLEGRO_DISPLAY *display = NULL;
		ALLEGRO_EVENT_QUEUE *event_queue = NULL;
		ALLEGRO_TIMER *timer = NULL;
		if (!al_init())
			return -1;
		if (!al_install_audio()) {
			return -1;
		}
		if (!al_init_acodec_addon()) {
			return -1;
		}
		if (!al_reserve_samples(1)) {
			return -1;
		}
		al_get_monitor_info(0, &Monitor_info);
		width = (int)Monitor_info.x2;
		height = (int)Monitor_info.y2;

		al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
		display = al_create_display(width, height);
		if (!display)
			return -1;
		sample = al_load_sample("dzwieki/main.wav");
		if (!sample) {
			return -1;
		}
		///////////// addony////////////////
		al_init_font_addon();
		al_install_mouse();
		al_init_ttf_addon();
		al_init_image_addon();
		al_init_primitives_addon();
		al_install_keyboard();
		//ladowanie bitmap/////
		al_set_window_title(display, "Priceless Paper");
		BMP_WEJSCIOWE = al_load_bitmap("wejsciowe/tlo_podstawowe2.png");
		BMP_NOWA_GRA = al_load_bitmap("wejsciowe/NOWA_GRA_WYBRANE.png");
		BMP_CREDITS = al_load_bitmap("wejsciowe/CREDITS.png");
		BMP_ZAKONCZ = al_load_bitmap("wejsciowe/ZAKONCZ_GRE.png");
		BMP_OPCJE = al_load_bitmap("wejsciowe/OPCJE.png");
		BMP_REKORDY = al_load_bitmap("wejsciowe/REKORDY.PNG");
		BMP_JEDEN_GRACZ = al_load_bitmap("wejsciowe/JEDEN_GRACZ.png");
		BMP_DWOCH_GRACZY = al_load_bitmap("wejsciowe/DWOCH_GRACZY.png");
		BMP_MUZYKA = al_load_bitmap("wejsciowe/MUZYKA.png");
		BMP_MUZYKA_STAN = al_load_bitmap("wejsciowe/WLACZONA.png");
		BMP_POSTAC_GORA = al_load_bitmap("studentbmp/polnoc.png");
		BMP_POSTAC_DOL = al_load_bitmap("studentbmp/poludnie.png");
		BMP_POSTAC_PRAWO = al_load_bitmap("studentbmp/prawo.png");
		BMP_POSTAC_LEWO = al_load_bitmap("studentbmp/lewo.png");
		BMP_POCISK = al_load_bitmap("studentbmp/atak_podstawowy.png");
		ikona = al_load_bitmap("wejsciowe/ikona.png");
		al_set_display_icon(display, ikona);
		al_hide_mouse_cursor(display);
		////////// rysowanie poczatkowego menu //////////////////
		al_draw_bitmap(BMP_WEJSCIOWE, 0, 0, 0);
		al_draw_bitmap(BMP_NOWA_GRA, width / 2 - 190, height / 2 - 40, 0);
		al_draw_bitmap(BMP_REKORDY, width / 2 - 190, height / 2 + 15, 0);
		al_draw_bitmap(BMP_OPCJE, width / 2 - 190, height / 2 + 70, 0);
		al_draw_bitmap(BMP_CREDITS, width / 2 - 190, height / 2 + 125, 0);
		al_draw_bitmap(BMP_ZAKONCZ, width / 2 - 190, height / 2 + 180, 0);
		///////////////////// queue + timer 
		event_queue = al_create_event_queue();
		timer = al_create_timer(1.0 / FPS);
	/////////////// eventy
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_start_timer(timer);
	al_flip_display();
	al_stop_samples();
	//al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
	while (!wyszedl && !opuscil_menu) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);
		if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_UP:
				keys[UP] = true;
				kierunek = 0;
				if (!zablokowane[0]) if (--menu[0] == -1) menu[0] = 0;
				if (zablokowane[0]) if (--menu[1] == -1) menu[1] = 0;
				if (zablokowane[0]) if (--menu[2] == -1) menu[2] = 0;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = true;
				kierunek = 1;
				if (!zablokowane[0]) if (++menu[0] == 5) menu[0] = 4;
				if (zablokowane[0]) if (++menu[1] == 2) menu[1] = 1;
				if (zablokowane[0]) if (++menu[2] == 2) menu[2] = 1;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = true;
				kierunek = 2;
				if (zapamietaj == 2) {
					if (muzyka_wlaczona) {
						al_destroy_bitmap(BMP_WEJSCIOWE);
						BMP_WEJSCIOWE = al_load_bitmap("wejsciowe/tlo_podstawowe2.png");
						al_draw_bitmap(BMP_WEJSCIOWE, 0, 0, 0);
						al_destroy_bitmap(BMP_MUZYKA);
						BMP_MUZYKA = al_load_bitmap("wejsciowe/MUZYKA.png");
						al_draw_bitmap(BMP_MUZYKA, width / 2 - 350, height / 2 - 40, 0);
						al_destroy_bitmap(BMP_MUZYKA_STAN);
						BMP_MUZYKA_STAN = al_load_bitmap("wejsciowe/WYLACZONA.png");
						al_draw_bitmap(BMP_MUZYKA_STAN, width / 2 - 50, height / 2 - 40, 0);
						muzyka_wlaczona = false;
						al_stop_samples();
						al_flip_display();
					}
				}
				break;

			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = true;
				kierunek = 3;
				if (zapamietaj == 2) {
					if (!muzyka_wlaczona) {
						al_destroy_bitmap(BMP_WEJSCIOWE);
						BMP_WEJSCIOWE = al_load_bitmap("wejsciowe/tlo_podstawowe2.png");
						al_draw_bitmap(BMP_WEJSCIOWE, 0, 0, 0);
						al_destroy_bitmap(BMP_MUZYKA);
						BMP_MUZYKA = al_load_bitmap("wejsciowe/MUZYKA.png");
						al_draw_bitmap(BMP_MUZYKA, width / 2 - 350, height / 2 - 40, 0);
						al_destroy_bitmap(BMP_MUZYKA_STAN);
						BMP_MUZYKA_STAN = al_load_bitmap("wejsciowe/WLACZONA.png");
						al_draw_bitmap(BMP_MUZYKA_STAN, width / 2 - 50, height / 2 - 40, 0);
						al_flip_display();
						al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
						muzyka_wlaczona = true;
					}
				}
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_ESCAPE:
				zliczaj_enter--;
				if (zapamietaj == 0 || zapamietaj == 2) zapamietaj = -10;
				if (zliczaj_enter == -1) {
					wyszedl = true;
				}
				if (zliczaj_enter == 0) {
					al_draw_bitmap(BMP_WEJSCIOWE, 0, 0, 0);
					zablokowane[0] = false;
					menu[1] = menu[2] = 0;
				}
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = false;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = false;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = false;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = false;
				break;
			case ALLEGRO_KEY_ENTER:
				zliczaj_enter++;
				switch (menu[0]) {
				case 0:
					if (!zablokowane[0]) {
						powrocil = false;
						zablokowane[0] = true;
					}
					al_draw_bitmap(BMP_WEJSCIOWE, 0, 0, 0);
					al_draw_bitmap(BMP_JEDEN_GRACZ, width / 2 - 300, height / 2 - 40, 0);
					al_draw_bitmap(BMP_DWOCH_GRACZY, width / 2 - 300, height / 2 + 15, 0);
					al_flip_display();
					zapamietaj = 0;
					if (zliczaj_enter > 1) {
						switch (menu[1]) {
						case 0: wybor_ilosci_graczy[0] = true; wybor_ilosci_graczy[1] = false; opuscil_menu = true; in_game = true; al_clear_to_color(al_map_rgb(0, 0, 0));  pre_start_game();  break;
							//	case 1:  wybor_ilosci_graczy[0] = false; wybor_ilosci_graczy[1] = true; rozpocznij_gre(); break;
						}
						zliczaj_enter--;
					}
					if (zliczaj_enter_wybor > 1) {
						opuscil_menu = true;
					}
					break;
				case 1: zliczaj_enter--; break;
				case 2:
					zapamietaj = 2;
					if (!zablokowane[0]) {
						powrocil = false;
						zablokowane[0] = true;
					}
					al_draw_bitmap(BMP_WEJSCIOWE, 0, 0, 0);
					al_draw_bitmap(BMP_MUZYKA, width / 2 - 350, height / 2 - 40, 0);
					al_draw_bitmap(BMP_MUZYKA_STAN, width / 2 - 50, height / 2 - 40, 0);
					al_flip_display();
					if (++zliczaj_enter > 1) zliczaj_enter = 1;
					break;

				case 3:  zliczaj_enter--;  break;
				case 4: wyszedl = true; break;
				}
			}
		}
		else if (ev.type == ALLEGRO_EVENT_TIMER) {
			if (keys[DOWN] == false && !zablokowane[0] && !opuscil_menu) {
				switch (menu[0]) {
				case 0:  al_destroy_bitmap(BMP_NOWA_GRA); al_destroy_bitmap(BMP_REKORDY);  BMP_NOWA_GRA = al_load_bitmap("wejsciowe/NOWA_GRA_WYBRANE.PNG"); BMP_REKORDY = al_load_bitmap("wejsciowe/REKORDY.png"); break;
				case 1:  al_destroy_bitmap(BMP_REKORDY); al_destroy_bitmap(BMP_NOWA_GRA); al_destroy_bitmap(BMP_OPCJE); BMP_REKORDY = al_load_bitmap("wejsciowe/REKORDY_WYBRANE.png"); BMP_NOWA_GRA = al_load_bitmap("wejsciowe/NOWA_GRA.png"); BMP_OPCJE = al_load_bitmap("wejsciowe/OPCJE.png"); break;
				case 2:  al_destroy_bitmap(BMP_CREDITS); al_destroy_bitmap(BMP_REKORDY); al_destroy_bitmap(BMP_OPCJE); BMP_OPCJE = al_load_bitmap("wejsciowe/OPCJE_WYBRANE.png"); BMP_REKORDY = al_load_bitmap("wejsciowe/REKORDY.png"); BMP_CREDITS = al_load_bitmap("wejsciowe/CREDITS.png"); break;
				case 3:  al_destroy_bitmap(BMP_OPCJE); al_destroy_bitmap(BMP_CREDITS); al_destroy_bitmap(BMP_ZAKONCZ); BMP_CREDITS = al_load_bitmap("wejsciowe/CREDITS_WYBRANE.png"); BMP_OPCJE = al_load_bitmap("wejsciowe/OPCJE.png"); BMP_ZAKONCZ = al_load_bitmap("wejsciowe/ZAKONCZ_GRE.png"); break;
				case 4:  al_destroy_bitmap(BMP_ZAKONCZ); al_destroy_bitmap(BMP_CREDITS);  BMP_ZAKONCZ = al_load_bitmap("wejsciowe/ZAKONCZ_GRE_WYBRANE.png"); BMP_CREDITS = al_load_bitmap("wejsciowe/CREDITS.png"); break;
				}
				al_draw_bitmap(BMP_NOWA_GRA, width / 2 - 190, height / 2 - 40, 0);
				al_draw_bitmap(BMP_REKORDY, width / 2 - 190, height / 2 + 15, 0);
				al_draw_bitmap(BMP_OPCJE, width / 2 - 190, height / 2 + 70, 0);
				al_draw_bitmap(BMP_CREDITS, width / 2 - 190, height / 2 + 125, 0);
				al_draw_bitmap(BMP_ZAKONCZ, width / 2 - 190, height / 2 + 180, 0);
			}
			if (keys[DOWN] == false && zablokowane[0] && zapamietaj == 0 && !opuscil_menu) {
				switch (menu[1]) {
				case 0:  al_destroy_bitmap(BMP_JEDEN_GRACZ); al_destroy_bitmap(BMP_DWOCH_GRACZY);  BMP_JEDEN_GRACZ = al_load_bitmap("wejsciowe/JEDEN_GRACZ_WYBRANE.PNG"); BMP_DWOCH_GRACZY = al_load_bitmap("wejsciowe/DWOCH_GRACZY.png"); break;
				case 1:  al_destroy_bitmap(BMP_JEDEN_GRACZ); al_destroy_bitmap(BMP_DWOCH_GRACZY);   BMP_JEDEN_GRACZ = al_load_bitmap("wejsciowe/JEDEN_GRACZ.PNG"); BMP_DWOCH_GRACZY = al_load_bitmap("wejsciowe/DWOCH_GRACZY_WYBRANE.png");  break;
				}
				al_draw_bitmap(BMP_JEDEN_GRACZ, width / 2 - 300, height / 2 - 40, 0);
				al_draw_bitmap(BMP_DWOCH_GRACZY, width / 2 - 300, height / 2 + 15, 0);
			}
			if (keys[DOWN] == false && zablokowane[0] && zapamietaj == 2 && !opuscil_menu) {

			}
			al_flip_display();
		}
	}
	al_destroy_bitmap(BMP_WEJSCIOWE);
	al_destroy_bitmap(BMP_NOWA_GRA);
	al_destroy_bitmap(BMP_CREDITS);
	al_destroy_bitmap(BMP_ZAKONCZ);
	al_destroy_bitmap(BMP_OPCJE);
	al_destroy_bitmap(BMP_REKORDY);
	al_destroy_bitmap(BMP_JEDEN_GRACZ);
	al_destroy_sample(sample);
	al_destroy_bitmap(BMP_DWOCH_GRACZY);
	al_destroy_bitmap(BMP_MUZYKA);
	al_destroy_bitmap(BMP_MUZYKA_STAN);
	al_destroy_bitmap(BMP_POSTAC_GORA);
	al_destroy_bitmap(BMP_POSTAC_DOL);
	al_destroy_bitmap(BMP_POSTAC_LEWO);
	al_destroy_bitmap(BMP_POSTAC_PRAWO);
	al_destroy_bitmap(BMP_POSTAC);
	al_destroy_bitmap(BMP_POZIOM);
	al_destroy_bitmap(BMP_POCISK);
	al_destroy_event_queue(event_queue);
}
