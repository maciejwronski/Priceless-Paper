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


	/////////////////////////////// KONFIGURACJA ////////////////////////////////
#define FPS 60
#define PREDKOSC_KUL 6
#define PREDKOSC_PRZECIWNIKOW 5
#define OPOZNIENIE_STRZELANIA 0.6f
#define MAX_BULLETS 100 // maksymalna ilosc pociskow na mapie
#define MAX_PRZECIWNIKOW 5
#define PREDKOSC_POSTACI 5
#define PRZECIWNICY_KIERUNEK 0.4f
#define OPOZNIENIE_STRZELANIA_PRZECIWNICY 0.5f
#define PRAWODOPOBIENSTWO_STRZALU_PRZECIWNIK 400
#define PRAWDOPODOBIENSTWO_RESPAWN_PRZECIWNIK 1
#define PRAWDOPODOBIENSTWO_RESPAWN_BOSS 10
	/////////////////////////////// KONFIGURACJA ////////////////////////////////

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
float DeltaTime = 1.0 / FPS;
int poziom = 1;
int enemies_killed = 0;
bool fiolka_zniszczona[5] = { false, false, false, false, false };
float tajmer_opoznienie_gracz = -1.0f;
float tajmer_sterowanie_przeciwnikiem[MAX_PRZECIWNIKOW] = { -1.0f };
float tajmer_opoznienie_strzelania[MAX_PRZECIWNIKOW] = { -1.0f };
enum KEYS { UP, DOWN, LEFT, RIGHT, SPACE };
enum KEYS1 { W, S, A, D };
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
ALLEGRO_BITMAP *BMP_TEKSTURA1 = NULL;
ALLEGRO_BITMAP *BMP_TEKSTURA2 = NULL;
ALLEGRO_BITMAP *BMP_TEKSTURA3 = NULL;
ALLEGRO_BITMAP *BMP_TEKSTURA4 = NULL;
ALLEGRO_BITMAP *BMP_TEKSTURA5 = NULL;
ALLEGRO_BITMAP *BMP_SEMESTR_OVER = NULL;
ALLEGRO_BITMAP *BMP_PRZECIWNIK = NULL;
ALLEGRO_BITMAP *BMP_PRZECIWNIK_BOSS = NULL;
ALLEGRO_BITMAP *BMP_POCISK_PRZECIWNIK = NULL;

// mjuzik //
ALLEGRO_SAMPLE *sample = NULL;
//

int width, height;

typedef struct obiekt {
	int x;
	int y;
	int alive;
	int kierunek;
	int czyj;
} obiekt;

obiekt bullets[MAX_BULLETS];

int add_bullet(int x, int y, int kierunek, int czyj_pocisk) // 0 - gracz 1 - komputer
{
	int i;

	for (i = 0; i < MAX_BULLETS; i++) {
		if (bullets[i].alive == 0) {
			bullets[i].x = x;
			bullets[i].y = y;
			bullets[i].kierunek = kierunek;
			bullets[i].czyj = czyj_pocisk;
			return i;
		}
	}

	return(0);
}

typedef struct przeciwnik {
	int x;
	int y;
	int alive;
	int lifes;
	int died;
	int boss;
} przeciwnik;

void create_enemy(przeciwnik Przeciwnik[]) {
	for (int i = 0; i < MAX_PRZECIWNIKOW; i++) {
		if (!Przeciwnik[i].alive && Przeciwnik[i].died != 1) {
			if (rand() % PRAWDOPODOBIENSTWO_RESPAWN_PRZECIWNIK == 0) {
				Przeciwnik[i].alive = 1;
				Przeciwnik[i].lifes = 1;
				Przeciwnik[i].x = (rand() % width) + 30;
				Przeciwnik[i].y = (rand() % height - 60) + 50;
				while (Przeciwnik[i].x >= width / 2 - 150  && Przeciwnik[i].x <= width / 2 + al_get_bitmap_width(BMP_TEKSTURA1) * 2 && Przeciwnik[i].y >= height - 150 - al_get_bitmap_width(BMP_TEKSTURA1)) {
					Przeciwnik[i].x = (rand() % width) + 30;
				}
			/*/	while (Przeciwnik[i].y >= height - 150 - al_get_bitmap_width(BMP_TEKSTURA1)) {
					Przeciwnik[i].y = (rand() % height - 60) + 50;
				}/*/

				if (rand() % PRAWDOPODOBIENSTWO_RESPAWN_BOSS == 0) {
					Przeciwnik[i].boss = 1;
					Przeciwnik[i].lifes = 2;
				}
			}
		}
	}
}

void draw_bullets(obiekt bullets[]) {
	int i;
	for (i = 0; i < MAX_BULLETS; i++) {
		if (bullets[i].alive == 1)
		{
			if (bullets[i].czyj == 0)
				al_draw_bitmap(BMP_POCISK, bullets[i].x, bullets[i].y, 0);
			if (bullets[i].czyj == 1)
				al_draw_bitmap(BMP_POCISK_PRZECIWNIK, bullets[i].x, bullets[i].y, 0);
			switch (bullets[i].kierunek) {
			case 0: bullets[i].y -= PREDKOSC_KUL; break;
			case 1: bullets[i].y += PREDKOSC_KUL; break;
			case 2: bullets[i].x -= PREDKOSC_KUL; break;
			case 3: bullets[i].x += PREDKOSC_KUL; break;
			}
		}
	}
}
void draw_enemies(przeciwnik Przeciwnik[]) {
	int i;
	for (i = 0; i < MAX_PRZECIWNIKOW; i++)
		if (Przeciwnik[i].alive == 1 && Przeciwnik[i].died == 0) {
			al_draw_bitmap(BMP_PRZECIWNIK, Przeciwnik[i].x, Przeciwnik[i].y, 0);
			if (Przeciwnik[i].boss == 1) {
				al_draw_bitmap(BMP_PRZECIWNIK_BOSS, Przeciwnik[i].x, Przeciwnik[i].y, 0);
			}
		}
}
przeciwnik Przeciwnik[MAX_PRZECIWNIKOW];



void rysuj_poziom(int ktory) {
	switch (ktory) {
	case 1: {
		BMP_WEJSCIOWE = al_load_bitmap("mapybmp/pierwsza.png");
		BMP_SYMBOL = al_load_bitmap("mapybmp/I.png");
		BMP_TEKSTURA1 = al_load_bitmap("mapybmp/fiolka.png");
		BMP_TEKSTURA2 = al_load_bitmap("mapybmp/fiolka.png");
		BMP_TEKSTURA3 = al_load_bitmap("mapybmp/fiolka.png");
		BMP_TEKSTURA4 = al_load_bitmap("mapybmp/fiolka.png");
		BMP_TEKSTURA5 = al_load_bitmap("mapybmp/fiolka.png");
		BMP_SEMESTR_OVER = al_load_bitmap("mapybmp/1_semestr_koniec.png");
		BMP_PRZECIWNIK = al_load_bitmap("przeciwnicy/calka.png");
		BMP_PRZECIWNIK_BOSS = al_load_bitmap("przeciwnicy/calka_boss.png");
		BMP_POCISK_PRZECIWNIK = al_load_bitmap("przeciwnicy/calka_pocisk.png");
		al_flip_display();
		break;
	}
	case 2: {
		BMP_WEJSCIOWE = al_load_bitmap("mapybmp/pierwsza.png");
		BMP_SYMBOL = al_load_bitmap("mapybmp/II.png");
		BMP_TEKSTURA1 = al_load_bitmap("mapybmp/fiolka.png");
		BMP_TEKSTURA2 = al_load_bitmap("mapybmp/fiolka.png");
		BMP_TEKSTURA3 = al_load_bitmap("mapybmp/fiolka.png");
		BMP_TEKSTURA4 = al_load_bitmap("mapybmp/fiolka.png");
		BMP_TEKSTURA5 = al_load_bitmap("mapybmp/fiolka.png");
		BMP_PRZECIWNIK = al_load_bitmap("przeciwnicy/calka.png");
		BMP_POCISK_PRZECIWNIK = al_load_bitmap("przeciwnicy/calka_pocisk.png");
		BMP_SEMESTR_OVER = al_load_bitmap("mapybmp/2_semestr_koniec.png");
		al_flip_display();
		break;
	}
	case 3: {
		BMP_WEJSCIOWE = al_load_bitmap("mapybmp/pierwsza.png");
		BMP_SYMBOL = al_load_bitmap("mapybmp/II.png");
		BMP_TEKSTURA1 = al_load_bitmap("mapybmp/fiolka.png");
		BMP_TEKSTURA2 = al_load_bitmap("mapybmp/fiolka.png");
		BMP_TEKSTURA3 = al_load_bitmap("mapybmp/fiolka.png");
		BMP_TEKSTURA4 = al_load_bitmap("mapybmp/fiolka.png");
		BMP_TEKSTURA5 = al_load_bitmap("mapybmp/fiolka.png");
		BMP_PRZECIWNIK = al_load_bitmap("przeciwnicy/calka.png");
		BMP_POCISK_PRZECIWNIK = al_load_bitmap("przeciwnicy/calka_pocisk.png");
		BMP_SEMESTR_OVER = al_load_bitmap("mapybmp/3_semestr_koniec.png");
		al_flip_display();
		break;
	}
	case 4: {
		BMP_WEJSCIOWE = al_load_bitmap("mapybmp/pierwsza.png");
		BMP_SYMBOL = al_load_bitmap("mapybmp/II.png");
		BMP_TEKSTURA1 = al_load_bitmap("mapybmp/fiolka.png");
		BMP_TEKSTURA2 = al_load_bitmap("mapybmp/fiolka.png");
		BMP_TEKSTURA3 = al_load_bitmap("mapybmp/fiolka.png");
		BMP_TEKSTURA4 = al_load_bitmap("mapybmp/fiolka.png");
		BMP_TEKSTURA5 = al_load_bitmap("mapybmp/fiolka.png");
		BMP_PRZECIWNIK = al_load_bitmap("przeciwnicy/calka.png");
		BMP_POCISK_PRZECIWNIK = al_load_bitmap("przeciwnicy/calka_pocisk.png");
		BMP_SEMESTR_OVER = al_load_bitmap("mapybmp/4_semestr_koniec.png");
		al_flip_display();
		break;
	}
	case 5: {
		BMP_WEJSCIOWE = al_load_bitmap("mapybmp/pierwsza.png");
		BMP_SYMBOL = al_load_bitmap("mapybmp/II.png");
		BMP_TEKSTURA1 = al_load_bitmap("mapybmp/fiolka.png");
		BMP_TEKSTURA2 = al_load_bitmap("mapybmp/fiolka.png");
		BMP_TEKSTURA3 = al_load_bitmap("mapybmp/fiolka.png");
		BMP_TEKSTURA4 = al_load_bitmap("mapybmp/fiolka.png");
		BMP_TEKSTURA5 = al_load_bitmap("mapybmp/fiolka.png");
		BMP_PRZECIWNIK = al_load_bitmap("przeciwnicy/calka.png");
		BMP_POCISK_PRZECIWNIK = al_load_bitmap("przeciwnicy/calka_pocisk.png");
		BMP_SEMESTR_OVER = al_load_bitmap("mapybmp/5_semestr_koniec.png");
		al_flip_display();
		break;
	}
	case 6: {
		BMP_WEJSCIOWE = al_load_bitmap("mapybmp/pierwsza.png");
		BMP_SYMBOL = al_load_bitmap("mapybmp/II.png");
		BMP_TEKSTURA1 = al_load_bitmap("mapybmp/fiolka.png");
		BMP_TEKSTURA2 = al_load_bitmap("mapybmp/fiolka.png");
		BMP_TEKSTURA3 = al_load_bitmap("mapybmp/fiolka.png");
		BMP_TEKSTURA4 = al_load_bitmap("mapybmp/fiolka.png");
		BMP_TEKSTURA5 = al_load_bitmap("mapybmp/fiolka.png");
		BMP_PRZECIWNIK = al_load_bitmap("przeciwnicy/calka.png");
		BMP_POCISK_PRZECIWNIK = al_load_bitmap("przeciwnicy/calka_pocisk.png");
		BMP_SEMESTR_OVER = al_load_bitmap("mapybmp/6_semestr_koniec.png");
		al_flip_display();
		break;
	}
	case 7: {
		BMP_WEJSCIOWE = al_load_bitmap("mapybmp/pierwsza.png");
		BMP_SYMBOL = al_load_bitmap("mapybmp/II.png");
		BMP_TEKSTURA1 = al_load_bitmap("mapybmp/fiolka.png");
		BMP_TEKSTURA2 = al_load_bitmap("mapybmp/fiolka.png");
		BMP_TEKSTURA3 = al_load_bitmap("mapybmp/fiolka.png");
		BMP_TEKSTURA4 = al_load_bitmap("mapybmp/fiolka.png");
		BMP_TEKSTURA5 = al_load_bitmap("mapybmp/fiolka.png");
		BMP_PRZECIWNIK = al_load_bitmap("przeciwnicy/calka.png");
		BMP_POCISK_PRZECIWNIK = al_load_bitmap("przeciwnicy/calka_pocisk.png");
		BMP_SEMESTR_OVER = al_load_bitmap("mapybmp/6_semestr_koniec.png");
		al_flip_display();
		break;
	}
	}
}
void clean_everything() {
	pos_x = width / 2 - 100;
	pos_y = height - 80;
	kierunek = 0;
	for (int i = 0; i < 5; i++) {
		fiolka_zniszczona[i] = false;
	}
	for (int i = 0; i < MAX_PRZECIWNIKOW; i++) {
		Przeciwnik[i].died = 0;
		tajmer_sterowanie_przeciwnikiem[i] = -1.0f;
		tajmer_opoznienie_strzelania[i] = -1.0f;
	}
	for(int j = 0; j < MAX_BULLETS; j++) {
		bullets[j].alive = 0;
	}
	al_flip_display();
}
bool collision(int pos_x1, int width_x1, int pos_x2, int pos_y1, int height_y1, int pos_y2, int height_y2) {
	if (pos_x1 + width_x1 >= pos_x2 && pos_x1 <= pos_x2 + width_x1 && pos_y1 + height_y1 >= pos_y2 && pos_y1 <= pos_y2 + height_y2)
		return true;
	else return false;
}
void pre_start_game() {
	ALLEGRO_EVENT_QUEUE *event_queue_stage = NULL;
	ALLEGRO_TIMER *timer_stage = NULL;
	srand(time(NULL));
	int i, temp;
	event_queue_stage = al_create_event_queue();
	timer_stage = al_create_timer(DeltaTime);
	al_start_timer(timer_stage);
	al_register_event_source(event_queue_stage, al_get_keyboard_event_source());
	al_register_event_source(event_queue_stage, al_get_timer_event_source(timer_stage));
	rysuj_poziom(1);
	int SZEROKOSC_POCISK = al_get_bitmap_width(BMP_POCISK);
	int SZEROKOSC_FIOLKA = al_get_bitmap_width(BMP_TEKSTURA1);
	int WYSOKOSC_POCISK = al_get_bitmap_height(BMP_POCISK);
	int WYSOKOSC_FIOLKA = al_get_bitmap_height(BMP_TEKSTURA1);
	int SZEROKOSC_SYMBOL = al_get_bitmap_width(BMP_SYMBOL);
	int WYSOKOSC_SYMBOL = al_get_bitmap_height(BMP_SYMBOL);
	int WYSOKOSC_LUDEK = al_get_bitmap_height(BMP_POSTAC_GORA);
	int SZEROKOSC_LUDEK = al_get_bitmap_width(BMP_POSTAC_GORA);
	int SZEROKOSC_PRZECIWNIK = al_get_bitmap_width(BMP_PRZECIWNIK);
	int WYSOKOSC_PRZECIWNIK = al_get_bitmap_height(BMP_PRZECIWNIK);
	int ostatnia_pozycja_gracz[2] = { pos_x, pos_y };
	int ostatnia_pozycja_komputer[MAX_PRZECIWNIKOW][2];
	int fiolki_x[5] = { width / 2 - 55, width / 2 - 55 , width / 2 - 55 + al_get_bitmap_width(BMP_TEKSTURA1),width / 2 - 55 + al_get_bitmap_width(BMP_TEKSTURA1) * 2 ,width / 2 - 55 + al_get_bitmap_width(BMP_TEKSTURA1) * 2 };
	int fiolki_y[5] = { height - 70, height - 90 - al_get_bitmap_width(BMP_TEKSTURA1),height - 90 - al_get_bitmap_width(BMP_TEKSTURA1),height - 90 - al_get_bitmap_width(BMP_TEKSTURA1),height - 70 };
	int SYMBOL[2] = { width / 2, height - 65 };
	clean_everything();
	while (opuscil_menu && in_game) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue_stage, &ev);
		if (tajmer_opoznienie_gracz > 0.0f)
			tajmer_opoznienie_gracz = tajmer_opoznienie_gracz - (DeltaTime);
		for (i = 0; i < MAX_PRZECIWNIKOW; i++)
			if (tajmer_opoznienie_strzelania[i] > 0.0f)
				tajmer_opoznienie_strzelania[i] = tajmer_opoznienie_strzelania[i] - (DeltaTime);
		for (i = 0; i < MAX_PRZECIWNIKOW; i++)
			if (tajmer_sterowanie_przeciwnikiem[i] > 0.0f)
				tajmer_sterowanie_przeciwnikiem[i] = tajmer_sterowanie_przeciwnikiem[i] - (DeltaTime);
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
				if (tajmer_opoznienie_gracz <= 0) {
					temp = add_bullet(pos_x, pos_y + 20, kierunek, 0);
					bullets[temp].alive = 1;
					tajmer_opoznienie_gracz = OPOZNIENIE_STRZELANIA;
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
				break;
			case ALLEGRO_KEY_ESCAPE:
				in_game = false;
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_TIMER) {
			al_draw_bitmap(BMP_WEJSCIOWE, 0, 0, 0);
			al_draw_bitmap(BMP_SYMBOL, SYMBOL[0], SYMBOL[1], 0);
			al_draw_bitmap(BMP_TEKSTURA1, fiolki_x[0], fiolki_y[0], 0);
			al_draw_bitmap(BMP_TEKSTURA2, fiolki_x[1], fiolki_y[1], 0);
			al_draw_bitmap(BMP_TEKSTURA3, fiolki_x[2], fiolki_y[2], 0);
			al_draw_bitmap(BMP_TEKSTURA4, fiolki_x[3], fiolki_y[3], 0);
			al_draw_bitmap(BMP_TEKSTURA5, fiolki_x[4], fiolki_y[4], 0);
			draw_bullets(bullets);
			draw_enemies(Przeciwnik);

			for (i = 0; i < MAX_PRZECIWNIKOW; i++)
			{
				if (Przeciwnik[i].alive)
				{
					int random_x[MAX_PRZECIWNIKOW];
					int random_y[MAX_PRZECIWNIKOW];
					int losowa[MAX_PRZECIWNIKOW];
					if (tajmer_sterowanie_przeciwnikiem[i] <= 0) {
						losowa[i] = rand() % 4;
						tajmer_sterowanie_przeciwnikiem[i] = PRZECIWNICY_KIERUNEK;
						random_x[i] = rand() % PREDKOSC_PRZECIWNIKOW;
						random_y[i] = rand() % PREDKOSC_PRZECIWNIKOW;
					}
					if (tajmer_opoznienie_strzelania[i] <= 0 && rand() % PRAWODOPOBIENSTWO_STRZALU_PRZECIWNIK == 0) {
						temp = add_bullet(Przeciwnik[i].x, Przeciwnik[i].y + 20, kierunek, 1);
						bullets[temp].alive = 1;
						tajmer_opoznienie_strzelania[i] = OPOZNIENIE_STRZELANIA_PRZECIWNICY;
					}
					ostatnia_pozycja_komputer[i][0] = Przeciwnik[i].x;
					ostatnia_pozycja_komputer[i][1] = Przeciwnik[i].y;
					switch (losowa[i]) {
					case 0: Przeciwnik[i].x += random_x[i]; break;
					case 1: Przeciwnik[i].y += random_y[i]; break;
					case 2: Przeciwnik[i].x -= random_x[i]; break;
					case 3:Przeciwnik[i].y -= random_y[i]; break;
					}
					if (Przeciwnik[i].x < 0) { Przeciwnik[i].x = 0; tajmer_sterowanie_przeciwnikiem[i] = -1.0f; }
					if (Przeciwnik[i].y < 0) { Przeciwnik[i].y = 0; tajmer_sterowanie_przeciwnikiem[i] = -1.0f; }
					if (Przeciwnik[i].y + WYSOKOSC_PRZECIWNIK >= height) { Przeciwnik[i].y = height - WYSOKOSC_PRZECIWNIK; 						tajmer_sterowanie_przeciwnikiem[i] = -1.0f; }
					if (Przeciwnik[i].x + SZEROKOSC_PRZECIWNIK >= width) {
						Przeciwnik[i].x = width - SZEROKOSC_PRZECIWNIK; tajmer_sterowanie_przeciwnikiem[i] = -1.0f;
					}
					if (!fiolka_zniszczona[0] && collision(Przeciwnik[i].x, SZEROKOSC_PRZECIWNIK, fiolki_x[0], Przeciwnik[i].y, WYSOKOSC_PRZECIWNIK, fiolki_y[0], WYSOKOSC_FIOLKA)) {
						Przeciwnik[i].x = ostatnia_pozycja_komputer[i][0];
						Przeciwnik[i].y = ostatnia_pozycja_komputer[i][1];
					}
					if (!fiolka_zniszczona[1] && collision(Przeciwnik[i].x, SZEROKOSC_PRZECIWNIK, fiolki_x[1], Przeciwnik[i].y, WYSOKOSC_PRZECIWNIK, fiolki_y[1], WYSOKOSC_FIOLKA)) {
						Przeciwnik[i].x = ostatnia_pozycja_komputer[i][0];
						Przeciwnik[i].y = ostatnia_pozycja_komputer[i][1];
					}
					if (!fiolka_zniszczona[2] && collision(Przeciwnik[i].x, SZEROKOSC_PRZECIWNIK, fiolki_x[2], Przeciwnik[i].y, WYSOKOSC_PRZECIWNIK, fiolki_y[2], WYSOKOSC_FIOLKA)) {
						Przeciwnik[i].x = ostatnia_pozycja_komputer[i][0];
						Przeciwnik[i].y = ostatnia_pozycja_komputer[i][1];
					}
					if (!fiolka_zniszczona[3] && collision(Przeciwnik[i].x, SZEROKOSC_PRZECIWNIK, fiolki_x[3], Przeciwnik[i].y, WYSOKOSC_PRZECIWNIK, fiolki_y[3], WYSOKOSC_FIOLKA)) {
						Przeciwnik[i].x = ostatnia_pozycja_komputer[i][0];
						Przeciwnik[i].y = ostatnia_pozycja_komputer[i][1];
					}
					if (!fiolka_zniszczona[4] && collision(Przeciwnik[i].x, SZEROKOSC_PRZECIWNIK, fiolki_x[4], Przeciwnik[i].y, WYSOKOSC_PRZECIWNIK, fiolki_y[4], WYSOKOSC_FIOLKA)) {
						Przeciwnik[i].x = ostatnia_pozycja_komputer[i][0];
						Przeciwnik[i].y = ostatnia_pozycja_komputer[i][1];
					}
					if (collision(Przeciwnik[i].x, SZEROKOSC_PRZECIWNIK, SYMBOL[0], Przeciwnik[i].y, WYSOKOSC_PRZECIWNIK, SYMBOL[1], WYSOKOSC_SYMBOL)) {
						Przeciwnik[i].x = ostatnia_pozycja_komputer[i][0];
						Przeciwnik[i].y = ostatnia_pozycja_komputer[i][1];
					}
					if (collision(Przeciwnik[i].x, SZEROKOSC_PRZECIWNIK, pos_x, Przeciwnik[i].y, WYSOKOSC_PRZECIWNIK, pos_y, WYSOKOSC_LUDEK)) {
						Przeciwnik[i].x = ostatnia_pozycja_komputer[i][0];
						Przeciwnik[i].y = ostatnia_pozycja_komputer[i][1];
						pos_x = ostatnia_pozycja_gracz[0];
						pos_y = ostatnia_pozycja_gracz[1];
					}
				}
			}
			for (i = 0; i < MAX_BULLETS; i++)
			{
				if (bullets[i].alive)
				{
					if (bullets[i].x < 0 || bullets[i].y < 0 || bullets[i].x > width || bullets[i].y > height)
						bullets[i].alive = 0;
					for (int j = 0; j < MAX_PRZECIWNIKOW; j++) {
						if (bullets[i].czyj != 1 && Przeciwnik[j].died == 0 && Przeciwnik[j].alive && collision(bullets[i].x, SZEROKOSC_POCISK, Przeciwnik[j].x, bullets[i].y, WYSOKOSC_POCISK, Przeciwnik[j].y, WYSOKOSC_PRZECIWNIK)) {
							bullets[i].alive = 0;
							if (--Przeciwnik[j].lifes == 0) {
								Przeciwnik[j].alive = 0;
								Przeciwnik[j].died = 1;
								enemies_killed++;
							}
							if (enemies_killed == MAX_PRZECIWNIKOW) {
								al_draw_bitmap(BMP_WEJSCIOWE, 0, 0, 0);
								al_draw_bitmap(BMP_SEMESTR_OVER, width / 3, height / 2, 0);
								al_flip_display();
								al_stop_timer(timer_stage);
								clean_everything();
								enemies_killed = 0;
								al_rest(10.0);
								al_start_timer(timer_stage);
								rysuj_poziom(++poziom);
							}
						}
					}
					if (bullets[i].czyj == 1 && collision(bullets[i].x, SZEROKOSC_POCISK, pos_x, bullets[i].y, WYSOKOSC_POCISK, pos_y, WYSOKOSC_LUDEK)) {
						bullets[i].alive = 0;
						in_game = false;
					}

					if (!fiolka_zniszczona[0] && collision(bullets[i].x, SZEROKOSC_POCISK, fiolki_x[0], bullets[i].y, WYSOKOSC_POCISK, fiolki_y[0], WYSOKOSC_FIOLKA)) {
						bullets[i].alive = 0;
						BMP_TEKSTURA1 = al_load_bitmap("mapybmp/fiolka_zniszczona.png");
						fiolka_zniszczona[0] = true;
					}
					if (!fiolka_zniszczona[1] && collision(bullets[i].x, SZEROKOSC_POCISK, fiolki_x[1], bullets[i].y, WYSOKOSC_POCISK, fiolki_y[1], WYSOKOSC_FIOLKA)) {
						bullets[i].alive = 0;
						BMP_TEKSTURA2 = al_load_bitmap("mapybmp/fiolka_zniszczona.png");
						fiolka_zniszczona[1] = true;
					}
					if (!fiolka_zniszczona[2] && collision(bullets[i].x, SZEROKOSC_POCISK, fiolki_x[2], bullets[i].y, WYSOKOSC_POCISK, fiolki_y[2], WYSOKOSC_FIOLKA)) {
						bullets[i].alive = 0;
						BMP_TEKSTURA3 = al_load_bitmap("mapybmp/fiolka_zniszczona.png");
						fiolka_zniszczona[2] = true;
					}
					if (!fiolka_zniszczona[3] && collision(bullets[i].x, SZEROKOSC_POCISK, fiolki_x[3], bullets[i].y, WYSOKOSC_POCISK, fiolki_y[3], WYSOKOSC_FIOLKA)) {
						bullets[i].alive = 0;
						BMP_TEKSTURA4 = al_load_bitmap("mapybmp/fiolka_zniszczona.png");
						fiolka_zniszczona[3] = true;
					}
					if (!fiolka_zniszczona[4] && collision(bullets[i].x, SZEROKOSC_POCISK, fiolki_x[4], bullets[i].y, WYSOKOSC_POCISK, fiolki_y[4], WYSOKOSC_FIOLKA)) {
						bullets[i].alive = 0;
						BMP_TEKSTURA5 = al_load_bitmap("mapybmp/fiolka_zniszczona.png");
						fiolka_zniszczona[4] = true;
					}
					if (collision(bullets[i].x, SZEROKOSC_POCISK, SYMBOL[0], bullets[i].y, WYSOKOSC_POCISK, SYMBOL[1], WYSOKOSC_SYMBOL)) {
						bullets[i].alive = 0;
						in_game = false;
					}
				}
			}
			ostatnia_pozycja_gracz[0] = pos_x;
			ostatnia_pozycja_gracz[1] = pos_y;
			pos_y -= keys[UP] * PREDKOSC_POSTACI;
			pos_y += keys[DOWN] * PREDKOSC_POSTACI;
			pos_x -= keys[LEFT] * PREDKOSC_POSTACI;
			pos_x += keys[RIGHT] * PREDKOSC_POSTACI;
			if (pos_y <= 0) pos_y = 0;
			if (pos_x <= 0) pos_x = 0;
			if (pos_y + WYSOKOSC_LUDEK >= height) pos_y = height - WYSOKOSC_LUDEK;
			if (pos_x + SZEROKOSC_LUDEK >= width) pos_x = width - SZEROKOSC_LUDEK;
			if (!fiolka_zniszczona[0] && collision(pos_x, SZEROKOSC_LUDEK, fiolki_x[0], pos_y, WYSOKOSC_LUDEK, fiolki_y[0], WYSOKOSC_FIOLKA)) {
				pos_x = ostatnia_pozycja_gracz[0];
				pos_y = ostatnia_pozycja_gracz[1];
			}
			if (!fiolka_zniszczona[1] && collision(pos_x, SZEROKOSC_LUDEK, fiolki_x[1], pos_y, WYSOKOSC_LUDEK, fiolki_y[1], WYSOKOSC_FIOLKA)) {
				pos_x = ostatnia_pozycja_gracz[0];
				pos_y = ostatnia_pozycja_gracz[1];
			}
			if (!fiolka_zniszczona[2] && collision(pos_x, SZEROKOSC_LUDEK, fiolki_x[2], pos_y, WYSOKOSC_LUDEK, fiolki_y[2], WYSOKOSC_FIOLKA)) {
				pos_x = ostatnia_pozycja_gracz[0];
				pos_y = ostatnia_pozycja_gracz[1];
			}
			if (!fiolka_zniszczona[3] && collision(pos_x, SZEROKOSC_LUDEK, fiolki_x[3], pos_y, WYSOKOSC_LUDEK, fiolki_y[3], WYSOKOSC_FIOLKA)) {
				pos_x = ostatnia_pozycja_gracz[0];
				pos_y = ostatnia_pozycja_gracz[1];
			}
			if (!fiolka_zniszczona[4] && collision(pos_x, SZEROKOSC_LUDEK, fiolki_x[4], pos_y, WYSOKOSC_LUDEK, fiolki_y[4], WYSOKOSC_FIOLKA)) {
				pos_x = ostatnia_pozycja_gracz[0];
				pos_y = ostatnia_pozycja_gracz[1];
			}
			if (collision(pos_x, SZEROKOSC_LUDEK, SYMBOL[0], pos_y, WYSOKOSC_LUDEK, SYMBOL[1], WYSOKOSC_SYMBOL)) {
				pos_x = ostatnia_pozycja_gracz[0];
				pos_y = ostatnia_pozycja_gracz[1];
			}
			switch (kierunek) {
			case 0:al_draw_bitmap(BMP_POSTAC_GORA, pos_x, pos_y, 0); break;
			case 1:al_draw_bitmap(BMP_POSTAC_DOL, pos_x, pos_y, 0); break;
			case 2:al_draw_bitmap(BMP_POSTAC_LEWO, pos_x, pos_y, 0); break;
			case 3: al_draw_bitmap(BMP_POSTAC_PRAWO, pos_x, pos_y, 0); break;
			}
			create_enemy(Przeciwnik);
			al_flip_display();
		}

	}
}

int main(void) {
	ALLEGRO_DISPLAY_MODE disp_data;
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
	timer = al_create_timer(DeltaTime);
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
	al_destroy_bitmap(BMP_TEKSTURA1);
	al_destroy_bitmap(BMP_TEKSTURA2);
	al_destroy_bitmap(BMP_TEKSTURA3);
	al_destroy_bitmap(BMP_TEKSTURA4);
	al_destroy_bitmap(BMP_TEKSTURA5);
	al_destroy_bitmap(BMP_SEMESTR_OVER);
	al_destroy_bitmap(BMP_POCISK);
	al_destroy_bitmap(BMP_PRZECIWNIK);
	al_destroy_event_queue(event_queue);
}