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
#define MAX_PRZECIWNIKOW 10
#define MAX_PRZECIWNIKOW_JEDNOCZESNIE MAX_PRZECIWNIKOW
#define PREDKOSC_POSTACI 5
#define PRZECIWNICY_KIERUNEK 0.4f
#define OPOZNIENIE_STRZELANIA_PRZECIWNICY 0.5f
#define PRAWODOPOBIENSTWO_STRZALU_PRZECIWNIK 400
#define PRAWDOPODOBIENSTWO_RESPAWN_PRZECIWNIK 1
#define PRAWDOPODOBIENSTWO_RESPAWN_BOSS 10

//////////////// BONUSY /////////////////////
#define CZAS_NIESMIERTELNOSCI 5

////////////////DEFINICJA OBIEKTOW NA MAPIE////////////////
// 0 - PUSTO
// 1- FIOLKA
////////////////////////////////// KONFIGURACJA ////////////////////////////////

	////////////inicjalizacja zmiennych//////////////
	bool wyszedl = false;
bool powrocil = true;
bool zablokowane[] = { false };
bool keys[5] = { false, false, false, false, false };
bool keys1[5] = { false, false, false, false, false };
int menu[] = { 0, 0, 0 };
int zliczaj_enter = 0;
int zapamietaj, jeszcze_jeden_enter;
bool muzyka_wlaczona = true;
bool ostatni_wybor_muzyka = true;
bool opuscil_menu = false;
bool in_game = false;
bool wybor_ilosci_graczy[2] = { false, false };
int zliczaj_enter_wybor = 0;
int pos_player[2][2] = { 0 };
int player_alive[2] = { true, true };
int kierunek[2] = { -1 };
float DeltaTime = 1.0 / FPS;
int poziom = 1;
int enemies_killed = 0;
int enemies_count = 0;
// cooldowns for players shoots, 0 - player1, 1 - player2
float player_timer_cooldown[2] = { -1.0f };
// cooldowns for enemy's, 0 - movement, 1 - shoot
float enemy_timer_cooldown[2][MAX_PRZECIWNIKOW] = { -1.0f };
enum KEYS { UP, DOWN, LEFT, RIGHT, SPACE };
enum KEYS1 { W, S, A, D, CAPS };

int objMap[12][28] =
{{0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{ 0,1,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
{ 0,1,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,0,0},
{ 0,1,1,1,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,1,0,0,1,0,0,0},
{ 0,1,0,0,0,0,0,0,0,1,0,1,1,1,0,0,0,0,0,1,1,1,0,1,0,0,0,0},
{ 0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0},
{ 0,1,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,0,1,0,0,0,0},
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,1,0,0,0},
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,0,0},
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,1,0,},
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,},

};

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
ALLEGRO_BITMAP *BMP_POSTAC_GORA_2 = NULL;
ALLEGRO_BITMAP *BMP_POSTAC_DOL_2 = NULL;
ALLEGRO_BITMAP *BMP_POSTAC_LEWO_2 = NULL;
ALLEGRO_BITMAP *BMP_POSTAC_PRAWO_2 = NULL;
ALLEGRO_BITMAP *BMP_POSTAC_2 = NULL;
ALLEGRO_BITMAP *BMP_POCISK_2 = NULL;
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

ALLEGRO_BITMAP *TEST = NULL;

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
bool collision(int pos_x1, int width_x1, int pos_x2, int pos_y1, int height_y1, int pos_y2, int height_y2) {
	if (pos_x1 + width_x1 >= pos_x2 && pos_x1 <= pos_x2 + width_x1 && pos_y1 + height_y1 >= pos_y2 && pos_y1 <= pos_y2 + height_y2)
		return true;
	else return false;
}
void create_enemy(przeciwnik Przeciwnik[]) {
	for (int i = 0; i < MAX_PRZECIWNIKOW; i++) {
		if ((enemies_count < MAX_PRZECIWNIKOW_JEDNOCZESNIE) && !Przeciwnik[i].alive && Przeciwnik[i].died != 1) {
			if (rand() % PRAWDOPODOBIENSTWO_RESPAWN_PRZECIWNIK == 0) {
				Przeciwnik[i].alive = 1;
				Przeciwnik[i].lifes = 1;
				enemies_count++;
				Przeciwnik[i].x = (rand() % width) + 30;
				Przeciwnik[i].y = (rand() % height - 60) + 50;
				for (int t = 0; t <13; t++) {
					for (int z = 0; z < 28; z++) {
						if (objMap[t][z] != 0 && collision(Przeciwnik[i].x, al_get_bitmap_width(BMP_PRZECIWNIK), z * 48, Przeciwnik[i].y, al_get_bitmap_height(BMP_PRZECIWNIK), t * 70, al_get_bitmap_width(BMP_TEKSTURA1))) {
							Przeciwnik[i].x = (rand() % width) + 30;
							Przeciwnik[i].y = (rand() % height - 60) + 50;
						}
					}
				}
				while (Przeciwnik[i].x >= width / 2 - 150  && Przeciwnik[i].x <= width / 2 + al_get_bitmap_width(BMP_TEKSTURA1) * 2 && Przeciwnik[i].y >= height - 150 - al_get_bitmap_width(BMP_TEKSTURA1)) {
					Przeciwnik[i].x = (rand() % width) + 30;
				}
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
			if (bullets[i].czyj == 2)
				al_draw_bitmap(BMP_POCISK_2, bullets[i].x, bullets[i].y, 0);
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
		BMP_SEMESTR_OVER = al_load_bitmap("mapybmp/1_semestr_koniec.png");
		BMP_PRZECIWNIK = al_load_bitmap("przeciwnicy/calka.png");
		BMP_PRZECIWNIK_BOSS = al_load_bitmap("przeciwnicy/calka_boss.png");
		BMP_POCISK_PRZECIWNIK = al_load_bitmap("przeciwnicy/calka_pocisk.png");
		TEST = al_load_bitmap("mapybmp/fiolka.png");
		al_flip_display();
		break;
	}
	case 2: {
		BMP_WEJSCIOWE = al_load_bitmap("mapybmp/pierwsza.png");
		BMP_TEKSTURA1 = al_load_bitmap("mapybmp/fiolka.png");
		BMP_SYMBOL = al_load_bitmap("mapybmp/II.png");
		BMP_PRZECIWNIK = al_load_bitmap("przeciwnicy/calka.png");
		BMP_POCISK_PRZECIWNIK = al_load_bitmap("przeciwnicy/calka_pocisk.png");
		BMP_SEMESTR_OVER = al_load_bitmap("mapybmp/2_semestr_koniec.png");
		al_flip_display();
		break;
	}
	case 3: {
		BMP_WEJSCIOWE = al_load_bitmap("mapybmp/pierwsza.png");
		BMP_TEKSTURA1 = al_load_bitmap("mapybmp/fiolka.png");
		BMP_SYMBOL = al_load_bitmap("mapybmp/II.png");
		BMP_PRZECIWNIK = al_load_bitmap("przeciwnicy/calka.png");
		BMP_POCISK_PRZECIWNIK = al_load_bitmap("przeciwnicy/calka_pocisk.png");
		BMP_SEMESTR_OVER = al_load_bitmap("mapybmp/3_semestr_koniec.png");
		al_flip_display();
		break;
	}
	case 4: {
		BMP_WEJSCIOWE = al_load_bitmap("mapybmp/pierwsza.png");
		BMP_TEKSTURA1 = al_load_bitmap("mapybmp/fiolka.png");
		BMP_SYMBOL = al_load_bitmap("mapybmp/II.png");
		BMP_PRZECIWNIK = al_load_bitmap("przeciwnicy/calka.png");
		BMP_POCISK_PRZECIWNIK = al_load_bitmap("przeciwnicy/calka_pocisk.png");
		BMP_SEMESTR_OVER = al_load_bitmap("mapybmp/4_semestr_koniec.png");
		al_flip_display();
		break;
	}
	case 5: {
		BMP_WEJSCIOWE = al_load_bitmap("mapybmp/pierwsza.png");
		BMP_TEKSTURA1 = al_load_bitmap("mapybmp/fiolka.png");
		BMP_SYMBOL = al_load_bitmap("mapybmp/II.png");
		BMP_PRZECIWNIK = al_load_bitmap("przeciwnicy/calka.png");
		BMP_POCISK_PRZECIWNIK = al_load_bitmap("przeciwnicy/calka_pocisk.png");
		BMP_SEMESTR_OVER = al_load_bitmap("mapybmp/5_semestr_koniec.png");
		al_flip_display();
		break;
	}
	case 6: {
		BMP_WEJSCIOWE = al_load_bitmap("mapybmp/pierwsza.png");
		BMP_TEKSTURA1 = al_load_bitmap("mapybmp/fiolka.png");
		BMP_SYMBOL = al_load_bitmap("mapybmp/II.png");
		BMP_PRZECIWNIK = al_load_bitmap("przeciwnicy/calka.png");
		BMP_POCISK_PRZECIWNIK = al_load_bitmap("przeciwnicy/calka_pocisk.png");
		BMP_SEMESTR_OVER = al_load_bitmap("mapybmp/6_semestr_koniec.png");
		al_flip_display();
		break;
	}
	case 7: {
		BMP_WEJSCIOWE = al_load_bitmap("mapybmp/pierwsza.png");
		BMP_TEKSTURA1 = al_load_bitmap("mapybmp/fiolka.png");
		BMP_SYMBOL = al_load_bitmap("mapybmp/II.png");
		BMP_PRZECIWNIK = al_load_bitmap("przeciwnicy/calka.png");
		BMP_POCISK_PRZECIWNIK = al_load_bitmap("przeciwnicy/calka_pocisk.png");
		BMP_SEMESTR_OVER = al_load_bitmap("mapybmp/6_semestr_koniec.png");
		al_flip_display();
		break;
	}
	}
}
void clean_everything() {
	if (wybor_ilosci_graczy[0] == true) {
		pos_player[0][0] = width / 2 - 100;
		pos_player[0][1] = height - 80;
		kierunek[0] = 0;
	}
	if (wybor_ilosci_graczy[1] == true) {
		pos_player[1][0] = width / 2 + 100;
		pos_player[1][1] = height - 80;
		kierunek[0] = 0;
		kierunek[1] = 0;
	}

	for (int j = 0; j < 2; j++) 
		for (int i = 0; i < MAX_PRZECIWNIKOW; i++) 
			enemy_timer_cooldown[j][i] = -1.0f;

	for (int i = 0; i < MAX_PRZECIWNIKOW; i++)
		Przeciwnik[i].died = 0;

	for(int j = 0; j < MAX_BULLETS; j++)
		bullets[j].alive = 0;

	al_flip_display();
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
	int last_pos_player[2][2] = { pos_player[0][0], pos_player[0][1], pos_player[1][0], pos_player[1][1] };
	if (wybor_ilosci_graczy[1] == true) {
		player_alive[1] = true;
	}
	else player_alive[1] = false;
	int ostatnia_pozycja_komputer[MAX_PRZECIWNIKOW][2];
	int SYMBOL[2] = { width / 2, height - 65 };
	clean_everything();
	while (opuscil_menu && in_game) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue_stage, &ev);
		if (player_timer_cooldown[0] > 0.0f)
			player_timer_cooldown[0] = player_timer_cooldown[0] - (DeltaTime);
		if (player_timer_cooldown[1] > 0.0f)
			player_timer_cooldown[1] = player_timer_cooldown[1] - (DeltaTime);
		for (i = 0; i < MAX_PRZECIWNIKOW; i++)
			if (enemy_timer_cooldown[1][i] > 0.0f)
				enemy_timer_cooldown[1][i] = enemy_timer_cooldown[1][i] - (DeltaTime);
		for (i = 0; i < MAX_PRZECIWNIKOW; i++)
			if (enemy_timer_cooldown[0][i] > 0.0f)
				enemy_timer_cooldown[0][i] = enemy_timer_cooldown[0][i] - (DeltaTime);
		if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (ev.keyboard.keycode)
			{
				if (wybor_ilosci_graczy[1] == true) {
			case ALLEGRO_KEY_W:
				keys1[W] = true;
				kierunek[1] = 0;
				break;
			case ALLEGRO_KEY_S:
				keys1[S] = true;
				kierunek[1] = 1;
				break;
			case ALLEGRO_KEY_A:
				keys1[A] = true;
				kierunek[1] = 2;
				break;
			case ALLEGRO_KEY_D:
				keys1[D] = true;
				kierunek[1] = 3;
				break;
			case ALLEGRO_KEY_CAPSLOCK:
				keys1[CAPS] = true;
				if (player_timer_cooldown[1] <= 0 && player_alive[1]) {
					temp = add_bullet(pos_player[1][0], pos_player[1][1] + 20, kierunek[1], 2);
					bullets[temp].alive = 1;
					player_timer_cooldown[1] = OPOZNIENIE_STRZELANIA;
				}
				break;
			}
			case ALLEGRO_KEY_UP:
				keys[UP] = true;
				kierunek[0] = 0;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = true;
				kierunek[0] = 1;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = true;
				kierunek[0] = 2;
				break;

			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = true;
				kierunek[0] = 3;
				break;
			case ALLEGRO_KEY_SPACE:
				keys[SPACE] = true;
				if (player_timer_cooldown[0] <= 0 && player_alive[0]) {
					temp = add_bullet(pos_player[0][0], pos_player[0][1] + 20, kierunek[0], 0);
					bullets[temp].alive = 1;
					player_timer_cooldown[0] = OPOZNIENIE_STRZELANIA;
				}
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch (ev.keyboard.keycode) {
				if (wybor_ilosci_graczy[1] == true) {
			case ALLEGRO_KEY_W:
				keys1[W] = false;
				break;
			case ALLEGRO_KEY_S:
				keys1[S] = false;
				break;
			case ALLEGRO_KEY_A:
				keys1[A] = false;
				break;
			case ALLEGRO_KEY_D:
				keys1[D] = false;
				break;
			case ALLEGRO_KEY_CAPSLOCK:
				keys1[CAPS] = false;
				break;
				}
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
			draw_bullets(bullets);
			draw_enemies(Przeciwnik);

			for (i = 0; i < MAX_PRZECIWNIKOW; i++)
			{
				if (Przeciwnik[i].alive)
				{
					int random_x[MAX_PRZECIWNIKOW];
					int random_y[MAX_PRZECIWNIKOW];
					int losowa[MAX_PRZECIWNIKOW];
					if (enemy_timer_cooldown[0][i] <= 0) {
						losowa[i] = rand() % 4;
						enemy_timer_cooldown[0][i] = PRZECIWNICY_KIERUNEK;
						random_x[i] = rand() % PREDKOSC_PRZECIWNIKOW;
						random_y[i] = rand() % PREDKOSC_PRZECIWNIKOW;
					}
					if (enemy_timer_cooldown[1][i] <= 0 && rand() % PRAWODOPOBIENSTWO_STRZALU_PRZECIWNIK == 0) {
						temp = add_bullet(Przeciwnik[i].x, Przeciwnik[i].y + 20, kierunek[0], 1);
						bullets[temp].alive = 1;
						enemy_timer_cooldown[1][i] = OPOZNIENIE_STRZELANIA_PRZECIWNICY;
					}
					ostatnia_pozycja_komputer[i][0] = Przeciwnik[i].x;
					ostatnia_pozycja_komputer[i][1] = Przeciwnik[i].y;
					switch (losowa[i]) {
					case 0: Przeciwnik[i].x += random_x[i]; break;
					case 1: Przeciwnik[i].y += random_y[i]; break;
					case 2: Przeciwnik[i].x -= random_x[i]; break;
					case 3:Przeciwnik[i].y -= random_y[i]; break;
					}
					for (int t = 0; t <13; t++) {
						for (int z = 0; z < 28; z++) {
							if (objMap[t][z] == 1 && collision(Przeciwnik[i].x, SZEROKOSC_PRZECIWNIK, z * 48, Przeciwnik[i].y, WYSOKOSC_PRZECIWNIK, t * 70, WYSOKOSC_FIOLKA)) {
								Przeciwnik[i].x = ostatnia_pozycja_komputer[i][0];
								Przeciwnik[i].y = ostatnia_pozycja_komputer[i][1];
							}
						}
					}
					if (Przeciwnik[i].x < 0) { Przeciwnik[i].x = 0; enemy_timer_cooldown[0][i] = -1.0f; }
					if (Przeciwnik[i].y < 0) { Przeciwnik[i].y = 0; enemy_timer_cooldown[0][i] = -1.0f; }
					if (Przeciwnik[i].y + WYSOKOSC_PRZECIWNIK >= height) { Przeciwnik[i].y = height - WYSOKOSC_PRZECIWNIK; enemy_timer_cooldown[0][i] = -1.0f; }
					if (Przeciwnik[i].x + SZEROKOSC_PRZECIWNIK >= width) {
						Przeciwnik[i].x = width - SZEROKOSC_PRZECIWNIK; enemy_timer_cooldown[0][i] = -1.0f;
					}
					if (collision(Przeciwnik[i].x, SZEROKOSC_PRZECIWNIK, SYMBOL[0], Przeciwnik[i].y, WYSOKOSC_PRZECIWNIK, SYMBOL[1], WYSOKOSC_SYMBOL)) {
						Przeciwnik[i].x = ostatnia_pozycja_komputer[i][0];
						Przeciwnik[i].y = ostatnia_pozycja_komputer[i][1];
					}
					if (collision(Przeciwnik[i].x, SZEROKOSC_PRZECIWNIK, pos_player[0][0], Przeciwnik[i].y, WYSOKOSC_PRZECIWNIK, pos_player[0][1], WYSOKOSC_LUDEK)) {
						Przeciwnik[i].x = ostatnia_pozycja_komputer[i][0];
						Przeciwnik[i].y = ostatnia_pozycja_komputer[i][1];
						pos_player[0][0] = last_pos_player[0][0];
						pos_player[0][1] = last_pos_player[0][1];
					}
					if (collision(Przeciwnik[i].x, SZEROKOSC_PRZECIWNIK, pos_player[1][0], Przeciwnik[i].y, WYSOKOSC_PRZECIWNIK, pos_player[1][1], WYSOKOSC_LUDEK)) {
						Przeciwnik[i].x = ostatnia_pozycja_komputer[i][0];
						Przeciwnik[i].y = ostatnia_pozycja_komputer[i][1];
						pos_player[1][0] = last_pos_player[1][0];
						pos_player[1][1] = last_pos_player[1][1];
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
								enemies_count--;
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
					if (bullets[i].czyj == 1 && collision(bullets[i].x, SZEROKOSC_POCISK, pos_player[0][0], bullets[i].y, WYSOKOSC_POCISK, pos_player[0][1], WYSOKOSC_LUDEK)) {
						bullets[i].alive = 0;
						player_alive[0] = false;
						pos_player[0][0] = -100;
						pos_player[0][1] = -100;
					}
					if (bullets[i].czyj == 1 && collision(bullets[i].x, SZEROKOSC_POCISK, pos_player[1][0], bullets[i].y, WYSOKOSC_POCISK, pos_player[1][1], WYSOKOSC_LUDEK)) {
						bullets[i].alive = 0;
						player_alive[1] = false;
						pos_player[1][0] = -100;
						pos_player[1][1] = -100;
					}
					for (int t = 0; t <13; t++) {
						for (int z = 0; z < 28; z++) {
							if (objMap[t][z] == 1 && collision(bullets[i].x, SZEROKOSC_POCISK, z * 48, bullets[i].y, WYSOKOSC_POCISK, t * 70, WYSOKOSC_FIOLKA)) {
								bullets[i].alive = 0;
								objMap[t][z] = 0;
							}
						}
					}
					if (collision(bullets[i].x, SZEROKOSC_POCISK, SYMBOL[0], bullets[i].y, WYSOKOSC_POCISK, SYMBOL[1], WYSOKOSC_SYMBOL)) {
						bullets[i].alive = 0;
						in_game = false;
					}
				}
			}
			last_pos_player[0][0] = pos_player[0][0];
			last_pos_player[0][1] = pos_player[0][1];
			last_pos_player[1][0] = pos_player[1][0];
			last_pos_player[1][1] = pos_player[1][1];
			if (player_alive[0]) {
				pos_player[0][1] -= keys[UP] * PREDKOSC_POSTACI;
				pos_player[0][1] += keys[DOWN] * PREDKOSC_POSTACI;
				pos_player[0][0] -= keys[LEFT] * PREDKOSC_POSTACI;
				pos_player[0][0] += keys[RIGHT] * PREDKOSC_POSTACI;
			}
			if (wybor_ilosci_graczy[1] == true && player_alive[1]) {
				pos_player[1][1] -= keys1[W] * PREDKOSC_POSTACI;
				pos_player[1][1] += keys1[S] * PREDKOSC_POSTACI;
				pos_player[1][0] -= keys1[A] * PREDKOSC_POSTACI;
				pos_player[1][0] += keys1[D] * PREDKOSC_POSTACI;
			}
			if (player_alive[0] && player_alive[1] && collision(pos_player[0][0], SZEROKOSC_LUDEK, pos_player[1][0], pos_player[0][1], WYSOKOSC_LUDEK, pos_player[1][1], WYSOKOSC_LUDEK)){
				pos_player[0][0] = last_pos_player[0][0];
				pos_player[0][1] = last_pos_player[0][1];
				pos_player[1][0] = last_pos_player[1][0];
				pos_player[1][1] = last_pos_player[1][1];
			}
			if (player_alive[0]) {
				for (int t = 0; t <13; t++) {
					for (int i = 0; i < 28 ; i++) {
						if (objMap[t][i] == 1 && collision(pos_player[0][0], SZEROKOSC_LUDEK, i*48, pos_player[0][1], WYSOKOSC_LUDEK, t*70, WYSOKOSC_FIOLKA)) {
							pos_player[0][0] = last_pos_player[0][0];
							pos_player[0][1] = last_pos_player[0][1];
						}
					}
				}
				if (pos_player[0][1] <= 0) pos_player[0][1] = 0;
				if (pos_player[0][0] <= 0) pos_player[0][0] = 0;
				if (pos_player[0][1] + WYSOKOSC_LUDEK >= height) pos_player[0][1] = height - WYSOKOSC_LUDEK;
				if (pos_player[0][0] + SZEROKOSC_LUDEK >= width) pos_player[0][0] = width - SZEROKOSC_LUDEK;
				if (collision(pos_player[0][0], SZEROKOSC_LUDEK, SYMBOL[0], pos_player[0][1], WYSOKOSC_LUDEK, SYMBOL[1], WYSOKOSC_SYMBOL)) {
					pos_player[0][0] = last_pos_player[0][0];
					pos_player[0][1] = last_pos_player[0][1];
				}
			}
			if (wybor_ilosci_graczy[1] == true && player_alive[1]) {
				for (int t = 0; t <13; t++) {
					for (int i = 0; i < 28; i++) {
						if (objMap[t][i] == 1 && collision(pos_player[1][0], SZEROKOSC_LUDEK, i * 48, pos_player[1][1], WYSOKOSC_LUDEK, t * 70, WYSOKOSC_FIOLKA)) {
							pos_player[1][0] = last_pos_player[1][0];
							pos_player[1][1] = last_pos_player[1][1];
						}
					}
				}
			if (pos_player[1][1] <= 0) pos_player[1][1] = 0;
			if (pos_player[1][0] <= 0) pos_player[1][0] = 0;
				if (pos_player[1][1] + WYSOKOSC_LUDEK >= height) pos_player[1][1] = height - WYSOKOSC_LUDEK;
				if (pos_player[1][0] + SZEROKOSC_LUDEK >= width) pos_player[1][0] = width - SZEROKOSC_LUDEK;
				if (collision(pos_player[1][0], SZEROKOSC_LUDEK, SYMBOL[0], pos_player[1][1], WYSOKOSC_LUDEK, SYMBOL[1], WYSOKOSC_SYMBOL)) {
					pos_player[1][0] = last_pos_player[1][0];
					pos_player[1][1] = last_pos_player[1][1];
				}
			}
			switch (kierunek[0]) {
			case 0:al_draw_bitmap(BMP_POSTAC_GORA, pos_player[0][0], pos_player[0][1], 0); break;
			case 1:al_draw_bitmap(BMP_POSTAC_DOL, pos_player[0][0], pos_player[0][1], 0); break;
			case 2:al_draw_bitmap(BMP_POSTAC_LEWO, pos_player[0][0], pos_player[0][1], 0); break;
			case 3: al_draw_bitmap(BMP_POSTAC_PRAWO, pos_player[0][0], pos_player[0][1], 0); break;
			}
			if (wybor_ilosci_graczy[1] == true) {
				switch (kierunek[1]) {
				case 0:al_draw_bitmap(BMP_POSTAC_GORA_2, pos_player[1][0], pos_player[1][1], 0); break;
				case 1:al_draw_bitmap(BMP_POSTAC_DOL_2, pos_player[1][0], pos_player[1][1], 0); break;
				case 2:al_draw_bitmap(BMP_POSTAC_LEWO_2, pos_player[1][0], pos_player[1][1], 0); break;
				case 3: al_draw_bitmap(BMP_POSTAC_PRAWO_2, pos_player[1][0], pos_player[1][1], 0); break;
				}
			}
			create_enemy(Przeciwnik);
			for (int t = 0; t < 13; t++) {

				for (int i = 0; i < 28; i++) {
					if (objMap[t][i] == 1)
						al_draw_bitmap(TEST, i*48, t*70, 0);
				}

			}
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
	BMP_POSTAC_GORA_2 = al_load_bitmap("studentbmp/polnoc_2.png");
	BMP_POSTAC_DOL_2 = al_load_bitmap("studentbmp/poludnie_2.png");
	BMP_POSTAC_PRAWO_2 = al_load_bitmap("studentbmp/prawo_2.png");
	BMP_POSTAC_LEWO_2 = al_load_bitmap("studentbmp/lewo_2.png");
	BMP_POCISK_2 = al_load_bitmap("studentbmp/atak_podstawowy2.png");
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
				if (!zablokowane[0]) if (--menu[0] == -1) menu[0] = 0;
				if (zablokowane[0]) if (--menu[1] == -1) menu[1] = 0;
				if (zablokowane[0]) if (--menu[2] == -1) menu[2] = 0;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = true;
				if (!zablokowane[0]) if (++menu[0] == 5) menu[0] = 4;
				if (zablokowane[0]) if (++menu[1] == 2) menu[1] = 1;
				if (zablokowane[0]) if (++menu[2] == 2) menu[2] = 1;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = true;
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
						case 0: wybor_ilosci_graczy[0] = true; wybor_ilosci_graczy[1] = false; opuscil_menu = true; in_game = true; al_clear_to_color(al_map_rgb(0, 0, 0)); pre_start_game(); break;
						case 1:  wybor_ilosci_graczy[0] = true; wybor_ilosci_graczy[1] = true; opuscil_menu = true; in_game = true; al_clear_to_color(al_map_rgb(0, 0, 0));  pre_start_game(); break;
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
	al_destroy_bitmap(BMP_POSTAC_GORA_2);
	al_destroy_bitmap(BMP_POSTAC_DOL_2);
	al_destroy_bitmap(BMP_POSTAC_LEWO_2);
	al_destroy_bitmap(BMP_POSTAC_PRAWO_2);
	al_destroy_bitmap(BMP_POSTAC_2);
	al_destroy_bitmap(BMP_POZIOM);
	al_destroy_bitmap(BMP_TEKSTURA1);
	al_destroy_bitmap(BMP_TEKSTURA2);
	al_destroy_bitmap(BMP_TEKSTURA3);
	al_destroy_bitmap(BMP_TEKSTURA4);
	al_destroy_bitmap(BMP_TEKSTURA5);
	al_destroy_bitmap(BMP_SEMESTR_OVER);
	al_destroy_bitmap(BMP_POCISK);
	al_destroy_bitmap(BMP_POCISK_2);
	al_destroy_bitmap(BMP_PRZECIWNIK);
	al_destroy_event_queue(event_queue);
}