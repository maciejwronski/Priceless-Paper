//////////////////////////////////// Maciej Wronski, Politechnika Gdanska ////////////////////////////////////////////

#include<iostream>
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
#include <math.h>

/////////////////////////////// CONFIGURATION ////////////////////////////////
#define FPS 60
#define SPEED_OF_BULLETS 6
#define SPEED_OF_ENEMIES 5
#define COOLDOWN_ON_SHOTS 0.6f
#define MAX_BULLETS 100 // MAX NUMBER OF BULLETS AT THE MAP
#define MAX_NUMBER_OF_ENEMIES 20
#define MAX_NUMBER_OF_ENEMIES_AT_THE_SAME_TIME 4
#define SPEED_OF_PLAYER 5
#define COOLDOWN_ON_ENEMIES_DIRECTION 0.4f
#define COOLDOWN_ON_ENEMIES_SHOTS 0.5f
#define PROBABILITY_THAT_ENEMY_SHOOTS 400
#define PROBABILITY_THAT_ENEMY_RESPAWNS 100
#define PROBABILITY_THAT_BOSS_RESPAWNS 5
#define POINTS_FOR_ENEMY 100
#define POINTS_FOR_BOSS 200
#define SIZE_FONT 30
#define MAX_BONUS 10
#define DEFAULT_WIDTH 1366
#define DEFAULT_HEIGHT 768
///////////////////////////////////////////////
//////////////// BONUSES //////////////////////
#define TIME_OF_BEEING_UNDEAD 5
#define FREEZE_ON_ENEMIES 4
///////////////////////////////////////////////
////////////////OBIECTS ON MAP////////////////
// 0 - NOTHING
// 1 - FLASK
//////////////////////////////////////////////
////////////////////////////////// CONFIGURATION ////////////////////////////////
////////////VARIABLES//////////////
bool left = false;
bool is_back = true;
bool locked[] = { false };
bool keys[5] = { false, false, false, false, false };
bool keys1[5] = { false, false, false, false, false };
int menu[] = { 0, 0, 0 };
int count_enter = 0;
int remember, enter_once_more_time;
bool music_on = true;
bool last_music_option = true;
bool left_menu = false;
bool in_game = false;
bool frozen = false;
bool immunity[2] = { false, false };
bool number_of_players[2] = { false, false };
int count_enter_choice = 0;
int pos_player[2][2] = { 0 };
int player_alive[2] = { true, true };
int player_lifes[2] = { 1,1 };
int direction[2] = { -1 };
float scale[2];
float DeltaTime = 1.0 / FPS;
int stage = 1;
int enemies_killed = 0;
int enemies_count = 0;
int global_score = 0;
int enemy_size[2], main_screen_size[2];
int bullets_size[2], bullets_size_enemy[2];
int texture_size[2];
int constants_to_set_graphics[3][3]; // 0 main menu
// cooldowns for players shoots, 0 - player1, 1 - player2, 2 - immunity_player1, 3 - immunity_player 2
float player_timer_cooldown[4] = { -1.0f };
// cooldowns for enemy's, 0 - movement, 1 - shoot
float enemy_timer_cooldown[2][MAX_NUMBER_OF_ENEMIES] = { -1.0f };
float freeze_time = -1.0f;
float immunity_time[2] = { -1.0f, -1.0f };
enum KEYS { UP, DOWN, LEFT, RIGHT, SPACE };
enum KEYS1 { W, S, A, D, CAPS };

int objMap[11][28] =
{{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{ 0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0},
{ 1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0},

};

// menu bitmaps
ALLEGRO_BITMAP *BMP_START = NULL;
ALLEGRO_BITMAP *BMP_NEWGAME = NULL;
ALLEGRO_BITMAP *BMP_CREDITS = NULL;
ALLEGRO_BITMAP *BMP_EXIT = NULL;
ALLEGRO_BITMAP *BMP_OPTIONS = NULL;
ALLEGRO_BITMAP *BMP_RECORDS = NULL;
ALLEGRO_BITMAP *icon = NULL;
ALLEGRO_BITMAP *BMP_ONE_PLAYER = NULL;
ALLEGRO_BITMAP *BMP_TWO_PLAYERS = NULL;
ALLEGRO_BITMAP *BMP_MUSIC = NULL;
ALLEGRO_BITMAP *BMP_MUSIC_CURRENT = NULL;
//character bitmaps//
ALLEGRO_BITMAP *BMP_CHARACTER_UP = NULL;
ALLEGRO_BITMAP *BMP_CHARACTER_DOWN = NULL;
ALLEGRO_BITMAP *BMP_CHARACTER_LEFT = NULL;
ALLEGRO_BITMAP *BMP_CHARACTER_RIGHT = NULL;
ALLEGRO_BITMAP *BMP_BULLET = NULL;
ALLEGRO_BITMAP *BMP_CHARACTER_UP_2 = NULL;
ALLEGRO_BITMAP *BMP_CHARACTER_DOWN_2 = NULL;
ALLEGRO_BITMAP *BMP_CHARACTER_LEFT_2 = NULL;
ALLEGRO_BITMAP *BMP_CHARACTER_RIGHT_2 = NULL;
ALLEGRO_BITMAP *BMP_BULLET_2 = NULL;
//////////
ALLEGRO_BITMAP *BMP_SYMBOL = NULL;
ALLEGRO_BITMAP *BMP_TEXTURE_1 = NULL;
ALLEGRO_BITMAP *BMP_TEXTURE_2 = NULL;
ALLEGRO_BITMAP *BMP_TEXTURE_3 = NULL;
ALLEGRO_BITMAP *BMP_TEXTURE_4 = NULL;
ALLEGRO_BITMAP *BMP_TEXTURE_5 = NULL;
ALLEGRO_BITMAP *BMP_SEMESTER_OVER = NULL;
ALLEGRO_BITMAP *BMP_ENEMY = NULL;
ALLEGRO_BITMAP *BMP_ENEMY_BOSS = NULL;
ALLEGRO_BITMAP *BMP_BULLET_ENEMY = NULL;
////// BONUS_BITMAPS
ALLEGRO_BITMAP *BMP_FREEZE = NULL;
ALLEGRO_BITMAP *BMP_UNDEAD = NULL;
ALLEGRO_BITMAP *BMP_DESTROY = NULL;
ALLEGRO_BITMAP *BMP_LIFE = NULL;
ALLEGRO_BITMAP *BMP_RENOVATION = NULL;

// music sample //
ALLEGRO_SAMPLE *sample = NULL;
// font

int width, height;

typedef struct obiekt {
	int x;
	int y;
	int alive;
	int direction;
	int whose;
} obiekt;

obiekt bullets[MAX_BULLETS];

int add_bullet(int x, int y, int kierunek, int whose_bullet){ // 0 - gracz 1 - komputer 2 -- gracz 2
	int i;

	for (i = 0; i < MAX_BULLETS; i++) {
		if (bullets[i].alive == 0) {
			bullets[i].x = x;
			bullets[i].y = y;
			bullets[i].direction = kierunek;
			bullets[i].whose = whose_bullet;
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
	int points;
} przeciwnik;

typedef struct bonus {
	int x;
	int y;
	int alive;
	float time = 5.0;
	int type;
	int width;
	int height;
}bonus;
bonus Bonus[MAX_BONUS];

int create_bonus(bonus Bonus[], int x, int y) {
	for (int i = 0; i < MAX_BONUS; i++) {
		if (Bonus[i].alive == 0) {
			Bonus[i].alive = 1;
			Bonus[i].x = x;
			Bonus[i].y = y;
			Bonus[i].type = rand()%5;
			if (Bonus[i].type == 0) {
				Bonus[i].width = al_get_bitmap_width(BMP_UNDEAD);
				Bonus[i].height = al_get_bitmap_height(BMP_UNDEAD);
			}
			else if (Bonus[i].type == 1) {
				Bonus[i].width = al_get_bitmap_width(BMP_DESTROY);
				Bonus[i].height = al_get_bitmap_height(BMP_DESTROY);
			}
			else if (Bonus[i].type == 2) {
				Bonus[i].width = al_get_bitmap_width(BMP_FREEZE);
				Bonus[i].height = al_get_bitmap_height(BMP_FREEZE);
			}
			else if (Bonus[i].type == 3) {
				Bonus[i].width = al_get_bitmap_width(BMP_LIFE);
				Bonus[i].height = al_get_bitmap_height(BMP_LIFE);
			}
			else if (Bonus[i].type == 4){
				Bonus[i].width = al_get_bitmap_width(BMP_RENOVATION);
				Bonus[i].height = al_get_bitmap_height(BMP_RENOVATION);

			}
			return i;
		}
	}
	return(0);
}
void draw_bonus(bonus Bonus[]) {
	for (int i = 0; i < MAX_BONUS; i++) {
		if (Bonus[i].alive == 1) {
			if (Bonus[i].type == 0)
				//al_draw_bitmap(BMP_UNDEAD, Bonus[i].x, Bonus[i].y, 0);
				al_draw_scaled_bitmap(BMP_UNDEAD, 0, 0, Bonus[i].width, Bonus[i].height, Bonus[i].x, Bonus[i].y, Bonus[i].width*scale[0], Bonus[i].height*scale[1], 0);
			else if (Bonus[i].type == 1)
				//al_draw_bitmap(BMP_DESTROY, Bonus[i].x, Bonus[i].y, 0);
				al_draw_scaled_bitmap(BMP_DESTROY, 0, 0, Bonus[i].width, Bonus[i].height, Bonus[i].x, Bonus[i].y, Bonus[i].width*scale[0], Bonus[i].height*scale[1], 0);
			else if (Bonus[i].type == 2)
				//al_draw_bitmap(BMP_FREEZE, Bonus[i].x, Bonus[i].y, 0);
				al_draw_scaled_bitmap(BMP_FREEZE, 0, 0, Bonus[i].width, Bonus[i].height, Bonus[i].x, Bonus[i].y, Bonus[i].width*scale[0], Bonus[i].height*scale[1], 0);
			else if (Bonus[i].type == 3)
				//al_draw_bitmap(BMP_LIFE, Bonus[i].x, Bonus[i].y, 0);
				al_draw_scaled_bitmap(BMP_LIFE, 0, 0, Bonus[i].width, Bonus[i].height, Bonus[i].x, Bonus[i].y, Bonus[i].width*scale[0], Bonus[i].height*scale[1], 0);
			else if (Bonus[i].type == 4)
				//al_draw_bitmap(BMP_RENOVATION, Bonus[i].x, Bonus[i].y, 0);
				al_draw_scaled_bitmap(BMP_RENOVATION, 0, 0, Bonus[i].width, Bonus[i].height, Bonus[i].x, Bonus[i].y, Bonus[i].width*scale[0], Bonus[i].height*scale[1], 0);
		}
	}
}
bool collision(int pos_x1, int width_x1, int pos_x2, int width_x2, int pos_y1, int height_y1, int pos_y2, int height_y2) {
	if (pos_x1 + width_x1 >= pos_x2 && pos_x1 <= pos_x2 + width_x2 && pos_y1 + height_y1 >= pos_y2 && pos_y1 <= pos_y2 + height_y2)
		return true;
	else return false;
}
void create_enemy(przeciwnik Przeciwnik[]) {
	for (int i = 0; i < MAX_NUMBER_OF_ENEMIES; i++) {
		if ((enemies_count < MAX_NUMBER_OF_ENEMIES_AT_THE_SAME_TIME) && !Przeciwnik[i].alive && Przeciwnik[i].died != 1) {
			if (rand() % PROBABILITY_THAT_ENEMY_RESPAWNS == 0) {
				Przeciwnik[i].alive = 1;
				Przeciwnik[i].lifes = 1;
				Przeciwnik[i].points = POINTS_FOR_ENEMY;
				enemies_count++;
				Przeciwnik[i].x = (rand() % width) + 30;
				Przeciwnik[i].y = (rand() % height - 60) + 50;
				for (int t = 0; t <11; t++) {
					for (int z = 0; z < 28; z++) {
						if (objMap[t][z] != 0 && collision(Przeciwnik[i].x, al_get_bitmap_width(BMP_ENEMY), z * 48*scale[0], al_get_bitmap_width(BMP_TEXTURE_1), Przeciwnik[i].y, al_get_bitmap_height(BMP_ENEMY), t * 70*scale[1], al_get_bitmap_height(BMP_TEXTURE_1))) {
							Przeciwnik[i].x = (rand() % width) + 30;
							Przeciwnik[i].y = (rand() % height - 60) + 50;
						}
					}
				}
				while (Przeciwnik[i].x >= width / 2 - 150  && Przeciwnik[i].x <= width / 2 + al_get_bitmap_width(BMP_TEXTURE_1) * 2 && Przeciwnik[i].y >= height - 150 - al_get_bitmap_width(BMP_TEXTURE_1)) {
					Przeciwnik[i].x = (rand() % width) + 30;
				}
				if (rand() % PROBABILITY_THAT_BOSS_RESPAWNS == 0) {
					Przeciwnik[i].boss = 1;
					Przeciwnik[i].lifes = 2;
					Przeciwnik[i].points = POINTS_FOR_BOSS;
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
			if (bullets[i].whose == 0)
				//al_draw_bitmap(BMP_BULLET, bullets[i].x, bullets[i].y, 0);
				al_draw_scaled_bitmap(BMP_BULLET, 0, 0, bullets_size[0], bullets_size[1], bullets[i].x, bullets[i].y, bullets_size[0]*scale[0], bullets_size[1]*scale[1], 0);
			if (bullets[i].whose == 1)
				//al_draw_bitmap(BMP_BULLET_ENEMY, bullets[i].x, bullets[i].y, 0);
				al_draw_scaled_bitmap(BMP_BULLET_ENEMY, 0, 0, bullets_size_enemy[0], bullets_size_enemy[1], bullets[i].x, bullets[i].y, bullets_size_enemy[0] * scale[0], bullets_size_enemy[1] * scale[1], 0);
			if (bullets[i].whose == 2)
				//al_draw_bitmap(BMP_BULLET_2, bullets[i].x, bullets[i].y, 0);
				al_draw_scaled_bitmap(BMP_BULLET_2, 0, 0, bullets_size[0], bullets_size[1], bullets[i].x, bullets[i].y, bullets_size[0] * scale[0], bullets_size[1] * scale[1], 0);
			switch (bullets[i].direction) {
			case 0: bullets[i].y -= SPEED_OF_BULLETS; break;
			case 1: bullets[i].y += SPEED_OF_BULLETS; break;
			case 2: bullets[i].x -= SPEED_OF_BULLETS; break;
			case 3: bullets[i].x += SPEED_OF_BULLETS; break;
			}
		}
	}
}
void draw_enemies(przeciwnik Przeciwnik[]) {
	int i;
	for (i = 0; i < MAX_NUMBER_OF_ENEMIES; i++) {
		if(Przeciwnik[i].alive && Przeciwnik[i].died == 0){
			if (Przeciwnik[i].boss == 0) 
				//al_draw_bitmap(BMP_ENEMY, Przeciwnik[i].x, Przeciwnik[i].y, 0);
				al_draw_scaled_bitmap(BMP_ENEMY, 0, 0, enemy_size[0], enemy_size[1], Przeciwnik[i].x, Przeciwnik[i].y, enemy_size[0] * scale[0], enemy_size[1] * scale[1], 0);
				else al_draw_scaled_bitmap(BMP_ENEMY_BOSS, 0, 0, enemy_size[0], enemy_size[1], Przeciwnik[i].x, Przeciwnik[i].y, enemy_size[0] * scale[0], enemy_size[1] * scale[1], 0);
					//al_draw_bitmap(BMP_ENEMY_BOSS, Przeciwnik[i].x, Przeciwnik[i].y, 0);

			}
		}
	}
przeciwnik Przeciwnik[MAX_NUMBER_OF_ENEMIES];

void draw_stage(int ktory) {
	switch (ktory) {
	case 1: {
		BMP_START = al_load_bitmap("mapybmp/pierwsza.png");
		BMP_SYMBOL = al_load_bitmap("mapybmp/I.png");
		BMP_TEXTURE_1 = al_load_bitmap("mapybmp/ksiazka.png");
		BMP_SEMESTER_OVER = al_load_bitmap("mapybmp/1_semestr_koniec.png");
		BMP_ENEMY = al_load_bitmap("przeciwnicy/calka.png");
		BMP_ENEMY_BOSS = al_load_bitmap("przeciwnicy/calka_boss.png");
		BMP_BULLET_ENEMY = al_load_bitmap("przeciwnicy/calka_pocisk.png");
		if (!BMP_START || !BMP_SYMBOL || !BMP_TEXTURE_1 || !BMP_SEMESTER_OVER || !BMP_ENEMY || !BMP_ENEMY_BOSS || !BMP_BULLET_ENEMY) {
			int error;
			printf("Semester 1 bitmaps didnt load correctly. Program will exit\nEnter any key to continue");
			scanf_s("%i", &error);
			exit(EXIT_FAILURE);
		}
		enemy_size[0] = al_get_bitmap_width(BMP_ENEMY);
		enemy_size[1] = al_get_bitmap_height(BMP_ENEMY);
		bullets_size_enemy[0] = al_get_bitmap_width(BMP_BULLET_ENEMY);
		bullets_size_enemy[1] = al_get_bitmap_height(BMP_BULLET_ENEMY);
		main_screen_size[0] = al_get_bitmap_width(BMP_START);
		main_screen_size[1] = al_get_bitmap_height(BMP_START);
		texture_size[0] = al_get_bitmap_width(BMP_TEXTURE_1);
		texture_size[1] = al_get_bitmap_height(BMP_TEXTURE_1);
		al_flip_display();
		break;
	}
	case 2: {
		BMP_START = al_load_bitmap("mapybmp/pierwsza.png");
		BMP_TEXTURE_1 = al_load_bitmap("mapybmp/komputer.png");
		BMP_SYMBOL = al_load_bitmap("mapybmp/II.png");
		BMP_ENEMY = al_load_bitmap("przeciwnicy/C.png");
		BMP_ENEMY_BOSS = al_load_bitmap("przeciwnicy/C_boss.png");
		BMP_SEMESTER_OVER = al_load_bitmap("mapybmp/2_semestr_koniec.png");
		BMP_BULLET_ENEMY = al_load_bitmap("przeciwnicy/C_pocisk.png");
		if (!BMP_START || !BMP_SYMBOL || !BMP_TEXTURE_1 || !BMP_SEMESTER_OVER || !BMP_ENEMY || !BMP_ENEMY_BOSS || !BMP_BULLET_ENEMY) {
			int error;
			printf("Semester 2 bitmaps didnt load correctly. Program will exit\nEnter any key to continue");
			scanf_s("%i", &error);
			exit(EXIT_FAILURE);
		}
		enemy_size[0] = al_get_bitmap_width(BMP_ENEMY);
		enemy_size[1] = al_get_bitmap_height(BMP_ENEMY);
		bullets_size_enemy[0] = al_get_bitmap_width(BMP_BULLET_ENEMY);
		bullets_size_enemy[1] = al_get_bitmap_height(BMP_BULLET_ENEMY);
		texture_size[0] = al_get_bitmap_width(BMP_TEXTURE_1);
		texture_size[1] = al_get_bitmap_height(BMP_TEXTURE_1);
		al_flip_display();
		break;
	}
	case 3: {
		BMP_START = al_load_bitmap("mapybmp/pierwsza.png");
		BMP_TEXTURE_1 = al_load_bitmap("mapybmp/fiolka.png");
		BMP_SYMBOL = al_load_bitmap("mapybmp/III.png");
		BMP_ENEMY = al_load_bitmap("przeciwnicy/fiolka.png");
		BMP_ENEMY_BOSS = al_load_bitmap("przeciwnicy/naukowiec.png");
		BMP_SEMESTER_OVER = al_load_bitmap("mapybmp/3_semestr_koniec.png");
		BMP_BULLET_ENEMY = al_load_bitmap("przeciwnicy/fiolka_pocisk.png");
		if (!BMP_START || !BMP_SYMBOL || !BMP_TEXTURE_1 || !BMP_SEMESTER_OVER || !BMP_ENEMY || !BMP_ENEMY_BOSS || !BMP_BULLET_ENEMY) {
			int error;
			printf("Semester 3 bitmaps didnt load correctly. Program will exit\nEnter any key to continue");
			scanf_s("%i", &error);
			exit(EXIT_FAILURE);
		}
		enemy_size[0] = al_get_bitmap_width(BMP_ENEMY);
		enemy_size[1] = al_get_bitmap_height(BMP_ENEMY);
		bullets_size_enemy[0] = al_get_bitmap_width(BMP_BULLET_ENEMY);
		bullets_size_enemy[1] = al_get_bitmap_height(BMP_BULLET_ENEMY);
		texture_size[0] = al_get_bitmap_width(BMP_TEXTURE_1);
		texture_size[1] = al_get_bitmap_height(BMP_TEXTURE_1);
		al_flip_display();
		break;
	}
	case 4: {
		BMP_START = al_load_bitmap("mapybmp/pierwsza.png");
		BMP_TEXTURE_1 = al_load_bitmap("mapybmp/opornik.png");
		BMP_SYMBOL = al_load_bitmap("mapybmp/IV.png");
		BMP_ENEMY= al_load_bitmap("przeciwnicy/fizyk.png");
		BMP_ENEMY_BOSS = al_load_bitmap("przeciwnicy/fizyk_boss.png");
		BMP_BULLET_ENEMY = al_load_bitmap("przeciwnicy/fizyk_pocisk.png");
		BMP_SEMESTER_OVER = al_load_bitmap("mapybmp/4_semestr_koniec.png");
		if (!BMP_START || !BMP_SYMBOL || !BMP_TEXTURE_1 || !BMP_SEMESTER_OVER || !BMP_ENEMY || !BMP_ENEMY_BOSS || !BMP_BULLET_ENEMY) {
			int error;
			printf("Semester 4 bitmaps didnt load correctly. Program will exit\nEnter any key to continue");
			scanf_s("%i", &error);
			exit(EXIT_FAILURE);
		}
		enemy_size[0] = al_get_bitmap_width(BMP_ENEMY);
		enemy_size[1] = al_get_bitmap_height(BMP_ENEMY);
		bullets_size_enemy[0] = al_get_bitmap_width(BMP_BULLET_ENEMY);
		bullets_size_enemy[1] = al_get_bitmap_height(BMP_BULLET_ENEMY);
		texture_size[0] = al_get_bitmap_width(BMP_TEXTURE_1);
		texture_size[1] = al_get_bitmap_height(BMP_TEXTURE_1);
		al_flip_display();
		break;
	}
	case 5: {
		BMP_START = al_load_bitmap("mapybmp/pierwsza.png");
		BMP_TEXTURE_1 = al_load_bitmap("mapybmp/cegla.png");
		BMP_SYMBOL = al_load_bitmap("mapybmp/V.png");
		BMP_ENEMY = al_load_bitmap("przeciwnicy/budowniczy.png");
		BMP_ENEMY_BOSS = al_load_bitmap("przeciwnicy/budowniczy_boss.png");
		BMP_BULLET_ENEMY = al_load_bitmap("przeciwnicy/budowniczy_pocisk.png");
		BMP_SEMESTER_OVER = al_load_bitmap("mapybmp/5_semestr_koniec.png");
		if (!BMP_START || !BMP_SYMBOL || !BMP_TEXTURE_1 || !BMP_SEMESTER_OVER || !BMP_ENEMY || !BMP_ENEMY_BOSS || !BMP_BULLET_ENEMY) {
			int error;
			printf("Semester 5 bitmaps didnt load correctly. Program will exit\nEnter any key to continue");
			scanf_s("%i", &error);
			exit(EXIT_FAILURE);
		}
		enemy_size[0] = al_get_bitmap_width(BMP_ENEMY);
		enemy_size[1] = al_get_bitmap_height(BMP_ENEMY);
		bullets_size_enemy[0] = al_get_bitmap_width(BMP_BULLET_ENEMY);
		bullets_size_enemy[1] = al_get_bitmap_height(BMP_BULLET_ENEMY);
		texture_size[0] = al_get_bitmap_width(BMP_TEXTURE_1);
		texture_size[1] = al_get_bitmap_height(BMP_TEXTURE_1);
		al_flip_display();
		break;
	}
	case 6: {
		BMP_START = al_load_bitmap("mapybmp/pierwsza.png");
		BMP_TEXTURE_1 = al_load_bitmap("mapybmp/robot.png");
		BMP_SYMBOL = al_load_bitmap("mapybmp/VI.png");
		BMP_ENEMY = al_load_bitmap("przeciwnicy/air.png");
		BMP_ENEMY_BOSS = al_load_bitmap("przeciwnicy/air_boss.png");
		BMP_BULLET_ENEMY = al_load_bitmap("przeciwnicy/air_pocisk.png");
		BMP_SEMESTER_OVER = al_load_bitmap("mapybmp/6_semestr_koniec.png");
		if (!BMP_START || !BMP_SYMBOL || !BMP_TEXTURE_1 || !BMP_SEMESTER_OVER || !BMP_ENEMY || !BMP_ENEMY_BOSS || !BMP_BULLET_ENEMY) {
			int error;
			printf("Semester 6 bitmaps didnt load correctly. Program will exit\nEnter any key to continue");
			scanf_s("%i", &error);
			exit(EXIT_FAILURE);
		}
		enemy_size[0] = al_get_bitmap_width(BMP_ENEMY);
		enemy_size[1] = al_get_bitmap_height(BMP_ENEMY);
		bullets_size_enemy[0] = al_get_bitmap_width(BMP_BULLET_ENEMY);
		bullets_size_enemy[1] = al_get_bitmap_height(BMP_BULLET_ENEMY);
		texture_size[0] = al_get_bitmap_width(BMP_TEXTURE_1);
		texture_size[1] = al_get_bitmap_height(BMP_TEXTURE_1);
		al_flip_display();
		break;
	}
	case 7: {
		BMP_START = al_load_bitmap("mapybmp/pierwsza.png");
		BMP_TEXTURE_1 = al_load_bitmap("mapybmp/dyplomowy.png");
		BMP_SYMBOL = al_load_bitmap("mapybmp/VII.png");
		BMP_ENEMY = al_load_bitmap("przeciwnicy/ostatni.png");
		BMP_ENEMY_BOSS = al_load_bitmap("przeciwnicy/ostatni_boss.png");
		BMP_BULLET_ENEMY = al_load_bitmap("przeciwnicy/ostatni_pocisk.png");
		BMP_SEMESTER_OVER = al_load_bitmap("mapybmp/6_semestr_koniec.png");
		if (!BMP_START || !BMP_SYMBOL || !BMP_TEXTURE_1 || !BMP_SEMESTER_OVER || !BMP_ENEMY || !BMP_ENEMY_BOSS || !BMP_BULLET_ENEMY) {
			int error;
			printf("Semester 7 bitmaps didnt load correctly. Program will exit\nEnter any key to continue");
			scanf_s("%i", &error);
			exit(EXIT_FAILURE);
		}
		enemy_size[0] = al_get_bitmap_width(BMP_ENEMY);
		enemy_size[1] = al_get_bitmap_height(BMP_ENEMY);
		bullets_size_enemy[0] = al_get_bitmap_width(BMP_BULLET_ENEMY);
		bullets_size_enemy[1] = al_get_bitmap_height(BMP_BULLET_ENEMY);
		texture_size[0] = al_get_bitmap_width(BMP_TEXTURE_1);
		texture_size[1] = al_get_bitmap_height(BMP_TEXTURE_1);
		al_flip_display();
		break;
	}
	}
}
void clean_everything() {
	if (number_of_players[0] == true) {
		pos_player[0][0] = width / 2 - 100;
		pos_player[0][1] = height - 80;
		direction[0] = 0;
	}
	if (number_of_players[1] == true) {
		pos_player[1][0] = width / 2 + 100;
		pos_player[1][1] = height - 80;
		direction[0] = 0;
		direction[1] = 0;
	}

	for (int j = 0; j < 2; j++) 
		for (int i = 0; i < MAX_NUMBER_OF_ENEMIES; i++)
			enemy_timer_cooldown[j][i] = -1.0f;

	for (int i = 0; i < MAX_NUMBER_OF_ENEMIES; i++) {
		Przeciwnik[i].died = 0;
		Przeciwnik[i].alive = 0;
		Przeciwnik[i].boss = 0;
	}

	for(int j = 0; j < MAX_BULLETS; j++)
		bullets[j].alive = 0;
	for (int i = 0; i < MAX_BONUS; i++)
		Bonus[i].alive = 0;
	al_flip_display();
}
void pre_start_game() {
	ALLEGRO_EVENT_QUEUE *event_queue_stage = NULL;
	ALLEGRO_TIMER *timer_stage = NULL;
	ALLEGRO_FONT *FONT_SCORE = al_load_font("fonts/georgia.ttf", SIZE_FONT, 0);
	int i, temp;
	event_queue_stage = al_create_event_queue();
	timer_stage = al_create_timer(DeltaTime);
	al_start_timer(timer_stage);
	al_register_event_source(event_queue_stage, al_get_keyboard_event_source());
	al_register_event_source(event_queue_stage, al_get_timer_event_source(timer_stage));
	draw_stage(1);
	int width_symbol = al_get_bitmap_width(BMP_SYMBOL);
	int height_symbol = al_get_bitmap_height(BMP_SYMBOL);
	int height_character = al_get_bitmap_height(BMP_CHARACTER_UP);
	int width_character = al_get_bitmap_width(BMP_CHARACTER_UP);
	bullets_size[0] = al_get_bitmap_width(BMP_BULLET);
	bullets_size[1] = al_get_bitmap_width(BMP_BULLET);
	int last_pos_player[2][2] = { pos_player[0][0], pos_player[0][1], pos_player[1][0], pos_player[1][1] };
	if (number_of_players[1] == true) {
		player_alive[1] = true;
	}
	else player_alive[1] = false;
	int pos_score_x = ceil(width * 2 / 3);
	int pos_symbol_y = floor(height * 0.92);
	int last_pos_comp[MAX_NUMBER_OF_ENEMIES][2];
	int SYMBOL[2] = { width / 2, pos_symbol_y };
	clean_everything();
	while (left_menu && in_game) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue_stage, &ev);
		if (player_timer_cooldown[0] > 0.0f)
			player_timer_cooldown[0] = player_timer_cooldown[0] - (DeltaTime);
		if (player_timer_cooldown[1] > 0.0f)
			player_timer_cooldown[1] = player_timer_cooldown[1] - (DeltaTime);
		if (freeze_time > 0.0f)
			freeze_time = freeze_time - (DeltaTime);
		if (freeze_time < 0.0f)
			frozen = false;
		if (player_timer_cooldown[2] > 0.0f)
			player_timer_cooldown[2] = player_timer_cooldown[2] - (DeltaTime);
		if (player_timer_cooldown[2] < 0.0f)
			immunity[0] = false;
		if (player_timer_cooldown[3] > 0.0f)
			player_timer_cooldown[3] = player_timer_cooldown[3] - (DeltaTime);
		if (player_timer_cooldown[3] < 0.0f)
			immunity[1] = false;
		for (i = 0; i < MAX_NUMBER_OF_ENEMIES; i++)
			if (enemy_timer_cooldown[1][i] > 0.0f)
				enemy_timer_cooldown[1][i] = enemy_timer_cooldown[1][i] - (DeltaTime);
		for (i = 0; i < MAX_NUMBER_OF_ENEMIES; i++)
			if (enemy_timer_cooldown[0][i] > 0.0f)
				enemy_timer_cooldown[0][i] = enemy_timer_cooldown[0][i] - (DeltaTime);
		if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (ev.keyboard.keycode)
			{
				if (number_of_players[1] == true) {
			case ALLEGRO_KEY_W:
				keys1[W] = true;
				direction[1] = 0;
				break;
			case ALLEGRO_KEY_S:
				keys1[S] = true;
				direction[1] = 1;
				break;
			case ALLEGRO_KEY_A:
				keys1[A] = true;
				direction[1] = 2;
				break;
			case ALLEGRO_KEY_D:
				keys1[D] = true;
				direction[1] = 3;
				break;
			case ALLEGRO_KEY_CAPSLOCK:
				keys1[CAPS] = true;
				if (player_timer_cooldown[1] <= 0 && player_alive[1]) {
					temp = add_bullet(pos_player[1][0], pos_player[1][1] + 20, direction[1], 2);
					bullets[temp].alive = 1;
					player_timer_cooldown[1] = COOLDOWN_ON_SHOTS;
				}
				break;
			}
			case ALLEGRO_KEY_UP:
				keys[UP] = true;
				direction[0] = 0;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = true;
				direction[0] = 1;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = true;
				direction[0] = 2;
				break;

			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = true;
				direction[0] = 3;
				break;
			case ALLEGRO_KEY_SPACE:
				keys[SPACE] = true;
				if (player_timer_cooldown[0] <= 0 && player_alive[0]) {
					temp = add_bullet(pos_player[0][0], pos_player[0][1] + 20, direction[0], 0);
					bullets[temp].alive = 1;
					player_timer_cooldown[0] = COOLDOWN_ON_SHOTS;
				}
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch (ev.keyboard.keycode) {
				if (number_of_players[1] == true) {
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
			//al_draw_bitmap(BMP_START, 0, 0, 0);
			al_draw_scaled_bitmap(BMP_START, 0, 0, al_get_bitmap_width(BMP_START), al_get_bitmap_height(BMP_START), 0, 0, al_get_bitmap_width(BMP_START)*scale[0], al_get_bitmap_height(BMP_START)*scale[1], 0);
			//al_draw_bitmap(BMP_SYMBOL, SYMBOL[0], SYMBOL[1], 0);
			al_draw_scaled_bitmap(BMP_SYMBOL, 0, 0, width_symbol, height_symbol, SYMBOL[0], SYMBOL[1], width_symbol*scale[0], height_symbol*scale[1], 0);
			for (i = 0; i < MAX_NUMBER_OF_ENEMIES; i++) {
				if (Przeciwnik[i].alive) {
					int random_x[MAX_NUMBER_OF_ENEMIES];
					int random_y[MAX_NUMBER_OF_ENEMIES];
					int losowa[MAX_NUMBER_OF_ENEMIES];
					if (enemy_timer_cooldown[0][i] <= 0 && !frozen) {
						losowa[i] = rand() % 4;
						enemy_timer_cooldown[0][i] = COOLDOWN_ON_ENEMIES_DIRECTION;
						random_x[i] = rand() % SPEED_OF_ENEMIES;
						random_y[i] = rand() % SPEED_OF_ENEMIES;
					}
					if (enemy_timer_cooldown[1][i] <= 0 && rand() % PROBABILITY_THAT_ENEMY_SHOOTS == 0) {
						temp = add_bullet(Przeciwnik[i].x, Przeciwnik[i].y + 20, direction[0], 1);
						bullets[temp].alive = 1;
						enemy_timer_cooldown[1][i] = COOLDOWN_ON_ENEMIES_SHOTS;
					}
					last_pos_comp[i][0] = Przeciwnik[i].x;
					last_pos_comp[i][1] = Przeciwnik[i].y;
					switch (losowa[i]) {
					case 0: Przeciwnik[i].x += random_x[i]; break;
					case 1: Przeciwnik[i].y += random_y[i]; break;
					case 2: Przeciwnik[i].x -= random_x[i]; break;
					case 3:Przeciwnik[i].y -= random_y[i]; break;
					}
					for (int t = 0; t < 11; t++) {
						for (int z = 0; z < 28; z++) {
							if (objMap[t][z] == 1 && collision(Przeciwnik[i].x, enemy_size[0], z * 48 * scale[0], texture_size[0], Przeciwnik[i].y, enemy_size[1], t * 70 * scale[1], texture_size[1])) {
								Przeciwnik[i].x = last_pos_comp[i][0];
								Przeciwnik[i].y = last_pos_comp[i][1];
							}
						}
					}
					for (int n = 0; n < MAX_NUMBER_OF_ENEMIES; n++) {
						if (Przeciwnik[n].alive && i != n && collision(Przeciwnik[i].x, enemy_size[0], Przeciwnik[n].x, enemy_size[0], Przeciwnik[i].y, enemy_size[1], Przeciwnik[n].y, enemy_size[1])) {
							Przeciwnik[i].x = last_pos_comp[i][0];
							Przeciwnik[i].y = last_pos_comp[i][1];
							Przeciwnik[n].x = last_pos_comp[n][0];
							Przeciwnik[n].y = last_pos_comp[n][1];
						}
					}
						if (Przeciwnik[i].x < 0) { Przeciwnik[i].x = 0; enemy_timer_cooldown[0][i] = -1.0f; }
						if (Przeciwnik[i].y < 0) { Przeciwnik[i].y = 0; enemy_timer_cooldown[0][i] = -1.0f; }
						if (Przeciwnik[i].y + enemy_size[1] >= height) { Przeciwnik[i].y = height - enemy_size[1];  enemy_timer_cooldown[0][i] = -1.0f; }
						if (Przeciwnik[i].x + enemy_size[0] >= width) {
							Przeciwnik[i].x = width - enemy_size[0]; enemy_timer_cooldown[0][i] = -1.0f;
						}
						if (collision(Przeciwnik[i].x, enemy_size[0], SYMBOL[0], width_symbol, Przeciwnik[i].y, enemy_size[1], SYMBOL[1], height_symbol)) {
							Przeciwnik[i].x = last_pos_comp[i][0];
							Przeciwnik[i].y = last_pos_comp[i][1];
						}
						if (collision(Przeciwnik[i].x, enemy_size[0], pos_player[0][0], width_character, Przeciwnik[i].y, enemy_size[1], pos_player[0][1], height_character)) {
							Przeciwnik[i].x = last_pos_comp[i][0];
							Przeciwnik[i].y = last_pos_comp[i][1];
							pos_player[0][0] = last_pos_player[0][0];
							pos_player[0][1] = last_pos_player[0][1];
						}
						if (collision(Przeciwnik[i].x, enemy_size[0], pos_player[1][0], width_character, Przeciwnik[i].y, enemy_size[1], pos_player[1][1], height_character)) {
							Przeciwnik[i].x = last_pos_comp[i][0];
							Przeciwnik[i].y = last_pos_comp[i][1];
							pos_player[1][0] = last_pos_player[1][0];
							pos_player[1][1] = last_pos_player[1][1];
						}
					}
				}

				for (i = 0; i < MAX_BULLETS; i++) {
					if (bullets[i].alive) {
						if (bullets[i].x < 0 || bullets[i].y < 0 || bullets[i].x > width || bullets[i].y > height)
							bullets[i].alive = 0;
						for (int n = 0; n < MAX_BULLETS; n++) {
							if (bullets[n].alive && (bullets[i].whose == 0 || bullets[n].whose == 0 || bullets[i].whose == 2 || bullets[n].whose == 2) && n != i && collision(bullets[i].x, bullets_size[0], bullets[n].x, bullets_size_enemy[0], bullets[i].y, bullets_size[1], bullets[n].y, bullets_size_enemy[1])) {
								bullets[i].alive = 0;
								bullets[n].alive = 0;
							}
							if (bullets[n].alive && bullets[i].whose == 1 && bullets[n].whose == 1 && n != i && collision(bullets[i].x, bullets_size_enemy[0], bullets[n].x, bullets_size_enemy[0], bullets[i].y, bullets_size_enemy[1], bullets[n].y, bullets_size_enemy[1])) {
								bullets[i].alive = 0;
								bullets[n].alive = 0;
							}
						}
						for (int j = 0; j < MAX_NUMBER_OF_ENEMIES; j++) {
							if (bullets[i].whose != 1 && Przeciwnik[j].died == 0 && Przeciwnik[j].alive && collision(bullets[i].x, bullets_size[0], Przeciwnik[j].x, enemy_size[0], bullets[i].y, bullets_size[1], Przeciwnik[j].y, enemy_size[1])) {
								bullets[i].alive = 0;
								if (--Przeciwnik[j].lifes == 0) {
									if (Przeciwnik[j].boss) {
										Przeciwnik[j].boss = 0;
										global_score += POINTS_FOR_BOSS;
										temp = create_bonus(Bonus, Przeciwnik[j].x, Przeciwnik[j].y);
										Bonus[temp].alive = 1;
									}
									else global_score += POINTS_FOR_ENEMY;
									Przeciwnik[j].alive = 0;
									Przeciwnik[j].died = 1;
									enemies_killed++;
									enemies_count--;
								}
								if (enemies_killed >= MAX_NUMBER_OF_ENEMIES) {
									al_draw_bitmap(BMP_START, 0, 0, 0);
									al_draw_bitmap(BMP_SEMESTER_OVER, width / 3, height / 2, 0);
									al_flip_display();
									al_stop_timer(timer_stage);
									clean_everything();
									enemies_killed = 0;
									al_rest(1.0);
									al_start_timer(timer_stage);
									draw_stage(++stage);
								}
							}
						}
						if (!immunity[0] && bullets[i].whose == 1 && collision(bullets[i].x, bullets_size[0], pos_player[0][0], width_character, bullets[i].y, bullets_size[1], pos_player[0][1], height_character) && player_lifes[0] == 1) {
							bullets[i].alive = 0;
							player_lifes[0] = 0;
							player_alive[0] = false;
							pos_player[0][0] = -100;
							pos_player[0][1] = -100;
						}
						if (!immunity[0] && bullets[i].whose == 1 && collision(bullets[i].x, bullets_size[0], pos_player[0][0], width_character, bullets[i].y, bullets_size[1], pos_player[0][1], height_character) && player_lifes[0] > 1) {
							pos_player[0][0] = width / 2 - 100;
							pos_player[0][1] = height - 80;
							player_lifes[0]--;
						}
						if (!immunity[1] && bullets[i].whose == 1 && collision(bullets[i].x, bullets_size[0], pos_player[1][0], width_character, bullets[i].y, bullets_size[1], pos_player[1][1], height_character) && player_lifes[1] == 1) {
							bullets[i].alive = 0;
							player_lifes[1] = 0;
							player_alive[1] = false;
							pos_player[1][0] = -100;
							pos_player[1][1] = -100;
						}
						if (!immunity[1] && bullets[i].whose == 1 && collision(bullets[i].x, bullets_size[0], pos_player[0][0], width_character, bullets[i].y, bullets_size[1], pos_player[0][1], height_character) && player_lifes[1] > 1) {
							pos_player[1][0] = width / 2 - 100;
							pos_player[1][1] = height - 80;
							player_lifes[1]--;
						}
						for (int t = 0; t < 11; t++) {
							for (int z = 0; z < 28; z++) {
								if (objMap[t][z] == 1 && collision(bullets[i].x, bullets_size[0], z * 48 * scale[0], texture_size[0], bullets[i].y, bullets_size[1], t * 70 * scale[1], texture_size[1])) {
									bullets[i].alive = 0;
									objMap[t][z] = 0;
								}
							}
						}
						if (collision(bullets[i].x, bullets_size[0], SYMBOL[0], width_symbol, bullets[i].y, bullets_size[1], SYMBOL[1], height_symbol)) {
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
					pos_player[0][1] -= keys[UP] * SPEED_OF_PLAYER;
					pos_player[0][1] += keys[DOWN] * SPEED_OF_PLAYER;
					pos_player[0][0] -= keys[LEFT] * SPEED_OF_PLAYER;
					pos_player[0][0] += keys[RIGHT] * SPEED_OF_PLAYER;
				}
				if (number_of_players[1] == true && player_alive[1]) {
					pos_player[1][1] -= keys1[W] * SPEED_OF_PLAYER;
					pos_player[1][1] += keys1[S] * SPEED_OF_PLAYER;
					pos_player[1][0] -= keys1[A] * SPEED_OF_PLAYER;
					pos_player[1][0] += keys1[D] * SPEED_OF_PLAYER;
				}
				if (player_alive[0] && player_alive[1] && collision(pos_player[0][0], width_character, pos_player[1][0], width_character, pos_player[0][1], height_character, pos_player[1][1], height_character)) {
					pos_player[0][0] = last_pos_player[0][0];
					pos_player[0][1] = last_pos_player[0][1];
					pos_player[1][0] = last_pos_player[1][0];
					pos_player[1][1] = last_pos_player[1][1];
				}
				if (player_alive[0]) {
					for (int t = 0; t < 11; t++) {
						for (int i = 0; i < 28; i++) {
							if (objMap[t][i] == 1 && collision(pos_player[0][0], width_character, i * 48 * scale[0], texture_size[0], pos_player[0][1], height_character, t * 70 * scale[1], texture_size[1])) {
								pos_player[0][0] = last_pos_player[0][0];
								pos_player[0][1] = last_pos_player[0][1];
							}
						}
					}
					if (pos_player[0][1] <= 0) pos_player[0][1] = 0;
					if (pos_player[0][0] <= 0) pos_player[0][0] = 0;
					if (pos_player[0][1] + height_character >= height) pos_player[0][1] = height - height_character;
					if (pos_player[0][0] + width_character >= width) pos_player[0][0] = width - width_character;
					if (collision(pos_player[0][0], width_character, SYMBOL[0], width_symbol, pos_player[0][1], height_character, SYMBOL[1], height_symbol)) {
						pos_player[0][0] = last_pos_player[0][0];
						pos_player[0][1] = last_pos_player[0][1];
					}
					for (int i = 0; i < MAX_BONUS; i++) {
						if (Bonus[i].alive && collision(pos_player[0][0], width_character, Bonus[i].x, Bonus[i].width, pos_player[0][1], height_character, Bonus[i].y, Bonus[i].height)) {
							switch (Bonus[i].type) {
							case 0: {
								if (player_timer_cooldown[2] <= 0) {
									immunity[0] = true;
									player_timer_cooldown[2] = TIME_OF_BEEING_UNDEAD;
								}
								Bonus[i].alive = 0;
								break;
							}
							case 1: {
								for (int j = 0; j < MAX_NUMBER_OF_ENEMIES; j++) {
									if (Przeciwnik[j].alive == 1 && Przeciwnik[j].died == 0) {
										Przeciwnik[j].alive = 0;
										Przeciwnik[j].died = 1;
										Przeciwnik[j].lifes = 0;
										Przeciwnik[j].boss = 0;
										Bonus[i].alive = 0;
										enemies_killed++;
										enemies_count--;
									}
								}
								if (enemies_killed == MAX_NUMBER_OF_ENEMIES) {
									al_draw_bitmap(BMP_START, 0, 0, 0);
									al_draw_bitmap(BMP_SEMESTER_OVER, width / 3, height / 2, 0);
									al_flip_display();
									al_stop_timer(timer_stage);
									clean_everything();
									enemies_killed = 0;
									al_rest(10.0);
									al_start_timer(timer_stage);
									draw_stage(++stage);
								}
								break;
							}
							case 2: {
								if (freeze_time <= 0) {
									frozen = true;
									freeze_time = FREEZE_ON_ENEMIES;
								}
								Bonus[i].alive = 0;
								break;
							}
							case 3: {
								Bonus[i].alive = 0;
								player_lifes[0]++;
								break;
							}
							case 4: {
								Bonus[i].alive = 0;
								for (int i = 13; i <= 15; i++) {
									if (objMap[9][i] == 0)
										objMap[9][i] = 1;
								}
								if (objMap[10][13] == 0)
									objMap[10][13] = 1;
								if (objMap[10][15] == 0)
									objMap[10][15] = 1;
								break;
							}
							}
						}
					}
				}
				if (number_of_players[1] == true && player_alive[1]) {
					for (int t = 0; t < 11; t++) {
						for (int i = 0; i < 28; i++) {
							if (objMap[t][i] == 1 && collision(pos_player[1][0], width_character, i * 48 * scale[0], texture_size[0], pos_player[1][1], height_character, t * 70 * scale[1], texture_size[1])) {
								pos_player[1][0] = last_pos_player[1][0];
								pos_player[1][1] = last_pos_player[1][1];
							}
						}
					}

					if (pos_player[1][1] <= 0) pos_player[1][1] = 0;
					if (pos_player[1][0] <= 0) pos_player[1][0] = 0;
					if (pos_player[1][1] + height_character >= height) pos_player[1][1] = height - height_character;
					if (pos_player[1][0] + width_character >= width) pos_player[1][0] = width - width_character;
					if (collision(pos_player[1][0], width_character, SYMBOL[0], width_symbol, pos_player[1][1], height_character, SYMBOL[1], height_symbol)) {
						pos_player[1][0] = last_pos_player[1][0];
						pos_player[1][1] = last_pos_player[1][1];
					}
					for (int i = 0; i < MAX_BONUS; i++) {
						if (Bonus[i].alive && collision(pos_player[1][0], width_character, Bonus[i].x, Bonus[i].width, pos_player[1][1], height_character, Bonus[i].y, Bonus[i].height)) {
							switch (Bonus[i].type) {
							case 0: {
								if (player_timer_cooldown[3] <= 0) {
									immunity[1] = true;
									player_timer_cooldown[3] = TIME_OF_BEEING_UNDEAD;
								}
								Bonus[i].alive = 0;
								break;
							}
							case 1: {
								for (int j = 0; j < MAX_NUMBER_OF_ENEMIES; j++) {
									if (Przeciwnik[j].alive == 1 && Przeciwnik[j].died == 0) {
										Przeciwnik[j].alive = 0;
										Przeciwnik[j].died = 1;
										Przeciwnik[j].lifes = 0;
										Przeciwnik[j].boss = 0;
										Bonus[i].alive = 0;
										enemies_killed++;
										enemies_count--;
									}
								}
								if (enemies_killed == MAX_NUMBER_OF_ENEMIES) {
									al_draw_bitmap(BMP_START, 0, 0, 0);
									al_draw_bitmap(BMP_SEMESTER_OVER, width / 3, height / 2, 0);
									al_flip_display();
									al_stop_timer(timer_stage);
									clean_everything();
									enemies_killed = 0;
									al_rest(10.0);
									al_start_timer(timer_stage);
									draw_stage(++stage);
								}
								break;
							}
							case 2: {
								if (freeze_time <= 0) {
									frozen = true;
									freeze_time = FREEZE_ON_ENEMIES;
								}
								Bonus[i].alive = 0;
								break;
							}
							case 3: {
								Bonus[i].alive = 0;
								player_lifes[1]++;
								break;
							}
							case 4: {
								Bonus[i].alive = 0;
								for (int i = 13; i <= 15; i++) {
									if (objMap[9][i] == 0)
										objMap[9][i] = 1;
								}
								if (objMap[10][13] == 0)
									objMap[10][13] = 1;
								if (objMap[10][15] == 0)
									objMap[10][15] = 1;
								break;
							}
							}
						}
					}
				}

				switch (direction[0]) {
					//case 0:al_draw_bitmap(BMP_CHARACTER_UP, pos_player[0][0], pos_player[0][1], 0); break;
					//case 1:al_draw_bitmap(BMP_CHARACTER_DOWN, pos_player[0][0], pos_player[0][1], 0); break;
					//case 2:al_draw_bitmap(BMP_CHARACTER_LEFT, pos_player[0][0], pos_player[0][1], 0); break;
					//case 3: al_draw_bitmap(BMP_CHARACTER_RIGHT, pos_player[0][0], pos_player[0][1], 0); break;
				case 0:al_draw_scaled_bitmap(BMP_CHARACTER_UP, 0, 0, width_character, height_character, pos_player[0][0], pos_player[0][1], width_character*scale[0], height_character*scale[1], 0); break;
				case 1:al_draw_scaled_bitmap(BMP_CHARACTER_DOWN, 0, 0, width_character, height_character, pos_player[0][0], pos_player[0][1], width_character*scale[0], height_character*scale[1], 0); break;
				case 2:al_draw_scaled_bitmap(BMP_CHARACTER_LEFT, 0, 0, width_character, height_character, pos_player[0][0], pos_player[0][1], width_character*scale[0], height_character*scale[1], 0); break;
				case 3:al_draw_scaled_bitmap(BMP_CHARACTER_RIGHT, 0, 0, width_character, height_character, pos_player[0][0], pos_player[0][1], width_character*scale[0], height_character*scale[1], 0); break;
				}
				if (number_of_players[1] == true) {
					switch (direction[1]) {
						//case 0:al_draw_bitmap(BMP_CHARACTER_UP_2, pos_player[1][0], pos_player[1][1], 0); break;
						//case 1:al_draw_bitmap(BMP_CHARACTER_DOWN_2, pos_player[1][0], pos_player[1][1], 0); break;
						//case 2:al_draw_bitmap(BMP_CHARACTER_LEFT_2, pos_player[1][0], pos_player[1][1], 0); break;
						//case 3: al_draw_bitmap(BMP_CHARACTER_RIGHT_2, pos_player[1][0], pos_player[1][1], 0); break;
					case 0:al_draw_scaled_bitmap(BMP_CHARACTER_UP_2, 0, 0, width_character, height_character, pos_player[1][0], pos_player[1][1], width_character*scale[0], height_character*scale[1], 0); break;
					case 1:al_draw_scaled_bitmap(BMP_CHARACTER_DOWN_2, 0, 0, width_character, height_character, pos_player[1][0], pos_player[1][1], width_character*scale[0], height_character*scale[1], 0); break;
					case 2:al_draw_scaled_bitmap(BMP_CHARACTER_LEFT_2, 0, 0, width_character, height_character, pos_player[1][0], pos_player[1][1], width_character*scale[0], height_character*scale[1], 0); break;
					case 3:al_draw_scaled_bitmap(BMP_CHARACTER_RIGHT_2, 0, 0, width_character, height_character, pos_player[1][0], pos_player[1][1], width_character*scale[0], height_character*scale[1], 0); break;
					}
				}
				draw_bullets(bullets);
				draw_enemies(Przeciwnik);
				draw_bonus(Bonus);
				if (number_of_players[1] == false)
					al_draw_textf(FONT_SCORE, al_map_rgb(255, 0, 0), pos_score_x, 0, 0, "Wynik: %d, Zyc: %d", global_score, player_lifes[0]);
				else
					al_draw_textf(FONT_SCORE, al_map_rgb(255, 0, 0), pos_score_x, 0, 0, "Wynik: %d, Zyc[1]: %d, Zyc[2]: %d", global_score, player_lifes[0], player_lifes[1]);
				create_enemy(Przeciwnik);
				for (int t = 0; t < 11; t++) {
					for (int i = 0; i < 28; i++) {
						if (objMap[t][i] == 1)
							//al_draw_bitmap(BMP_TEXTURE_1, i * 48, t * 70, 0);
							al_draw_scaled_bitmap(BMP_TEXTURE_1, 0, 0, texture_size[0], texture_size[1], i * 48 * scale[0], t * 70 * scale[1], texture_size[0] * scale[0], texture_size[1] * scale[1], 0);
					}
				}
				al_flip_display();
			}

	}
}

int main(void) {
	ALLEGRO_MONITOR_INFO Monitor_info;
	ALLEGRO_DISPLAY *display = NULL;

	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	if (!al_init()){
		int error;
		printf("Allegro didnt load correctly. Program will exit\nPress any key to continue");
		scanf_s("%i", &error);
		return -1;
	}
	if (!al_install_audio()) {
		int error;
		printf("install_audio didnt  load correctly. Program will exit\nPress any key to continue");
		scanf_s("%i", &error);
		return -1;
	}
	if (!al_init_acodec_addon()) {
		int error;
		printf("Codec addon didnt load correctly. Program will exit\nPress any key to continue");
		scanf_s("%i", &error);
		return -1;
	}
	if (!al_reserve_samples(1)) {
		int error;
		printf("Reserve samples didnt load correctly. Program will exit\nPress any key to continue");
		scanf_s("%i", &error);
		return -1;
	}
	al_get_monitor_info(0, &Monitor_info);
	width = (int)Monitor_info.x2;
	height = (int)Monitor_info.y2;
	scale[0] = (float)width / DEFAULT_WIDTH;
	scale[1] = (float)height / DEFAULT_HEIGHT;
		al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
	display = al_create_display(width, height);
	if (!display) {
		int error;
		printf("Display didnt load correctly. Program will exit\nPress any key to continue");
		scanf_s("%i", &error);
		return -1;
	}
	sample = al_load_sample("dzwieki/s.wav");
	if (!sample) {
		int error;
		printf("Sample didnt load correctly. Program will exit\nPress any key to continue");
		scanf_s("%i", &error);
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
	BMP_START = al_load_bitmap("wejsciowe/tlo_podstawowe2.png");
	BMP_NEWGAME = al_load_bitmap("wejsciowe/NOWA_GRA_WYBRANE.png");
	BMP_CREDITS = al_load_bitmap("wejsciowe/CREDITS.png");
	BMP_EXIT = al_load_bitmap("wejsciowe/ZAKONCZ_GRE.png");
	BMP_OPTIONS = al_load_bitmap("wejsciowe/OPCJE.png");
	BMP_RECORDS = al_load_bitmap("wejsciowe/REKORDY.PNG");
	BMP_ONE_PLAYER = al_load_bitmap("wejsciowe/JEDEN_GRACZ.png");
	BMP_TWO_PLAYERS = al_load_bitmap("wejsciowe/DWOCH_GRACZY.png");
	BMP_MUSIC = al_load_bitmap("wejsciowe/MUZYKA.png");
	BMP_MUSIC_CURRENT = al_load_bitmap("wejsciowe/WLACZONA.png");
	BMP_CHARACTER_UP = al_load_bitmap("studentbmp/polnoc.png");
	BMP_CHARACTER_DOWN = al_load_bitmap("studentbmp/poludnie.png");
	BMP_CHARACTER_RIGHT = al_load_bitmap("studentbmp/prawo.png");
	BMP_CHARACTER_LEFT = al_load_bitmap("studentbmp/lewo.png");
	BMP_BULLET = al_load_bitmap("studentbmp/atak_podstawowy.png");
	BMP_CHARACTER_UP_2 = al_load_bitmap("studentbmp/polnoc_2.png");
	BMP_CHARACTER_DOWN_2 = al_load_bitmap("studentbmp/poludnie_2.png");
	BMP_CHARACTER_RIGHT_2 = al_load_bitmap("studentbmp/prawo_2.png");
	BMP_CHARACTER_LEFT_2 = al_load_bitmap("studentbmp/lewo_2.png");
	BMP_BULLET_2 = al_load_bitmap("studentbmp/atak_podstawowy2.png");
	BMP_FREEZE = al_load_bitmap("bonusybmp/freeze.png");
	BMP_UNDEAD = al_load_bitmap("bonusybmp/niesmiertelnosc.png");
	BMP_DESTROY = al_load_bitmap("bonusybmp/zniszcz.png");
	BMP_LIFE = al_load_bitmap("bonusybmp/zycie.png");
	BMP_RENOVATION = al_load_bitmap("bonusybmp/odnowa.png");
	ALLEGRO_FONT *FONT_SCORE = al_load_font("fonts/georgia.ttf", 24, 0);
	icon = al_load_bitmap("wejsciowe/ikona.png");
	if (!icon || !FONT_SCORE || !BMP_BULLET || !BMP_START || !BMP_NEWGAME || !BMP_CREDITS || !BMP_EXIT || !BMP_EXIT || !BMP_OPTIONS || !BMP_RECORDS || !BMP_ONE_PLAYER || !BMP_TWO_PLAYERS || !BMP_MUSIC || !BMP_MUSIC_CURRENT || !BMP_CHARACTER_UP || !BMP_CHARACTER_DOWN || !BMP_CHARACTER_UP || !BMP_CHARACTER_LEFT || !BMP_CHARACTER_RIGHT || !BMP_CHARACTER_UP_2 || !BMP_CHARACTER_DOWN_2 || !BMP_CHARACTER_LEFT_2 || !BMP_CHARACTER_RIGHT_2 || !BMP_BULLET || !BMP_BULLET_2 || !BMP_UNDEAD ||!BMP_FREEZE || !BMP_LIFE || !BMP_DESTROY || !BMP_RENOVATION) {
		int error;
		printf("One of bitmaps didnt load correctly in menu. Program will exit\nPress any key to continue");
		scanf_s("%i", &error);
		return -1;
	}
	al_set_display_icon(display, icon);
	al_hide_mouse_cursor(display);
	////////// rysowanie poczatkowego menu //////////////////
	al_draw_bitmap(BMP_START, 0, 0, 0);
	///////////////////// queue + timer 
	event_queue = al_create_event_queue();
	timer = al_create_timer(DeltaTime);
	/////////////// eventy
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_start_timer(timer);
	al_flip_display();
	al_stop_samples();
	al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
	srand(time(NULL));
	constants_to_set_graphics[0][0] = width*0.36;
	constants_to_set_graphics[0][1] = height*0.44;
	constants_to_set_graphics[0][2] = height*0.07; // menu
	constants_to_set_graphics[1][0] = width*0.28; // music
	constants_to_set_graphics[2][0] = width*0.24;
	constants_to_set_graphics[2][1] = width*0.46;
	while (!left && !left_menu) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);
		if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_UP:
				keys[UP] = true;
				if (!locked[0]) if (--menu[0] == -1) menu[0] = 0;
				if (locked[0]) if (--menu[1] == -1) menu[1] = 0;
				if (locked[0]) if (--menu[2] == -1) menu[2] = 0;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = true;
				if (!locked[0]) if (++menu[0] == 5) menu[0] = 4;
				if (locked[0]) if (++menu[1] == 2) menu[1] = 1;
				if (locked[0]) if (++menu[2] == 2) menu[2] = 1;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = true;
				if (remember == 2) {
					if (music_on) {
						BMP_START = al_load_bitmap("wejsciowe/tlo_podstawowe2.png");
						BMP_MUSIC = al_load_bitmap("wejsciowe/MUZYKA.png");
						BMP_MUSIC_CURRENT = al_load_bitmap("wejsciowe/WYLACZONA.png");
						al_draw_scaled_bitmap(BMP_START, 0, 0, al_get_bitmap_width(BMP_START), al_get_bitmap_height(BMP_START), 0, 0, al_get_bitmap_width(BMP_START)*scale[0], al_get_bitmap_height(BMP_START)*scale[1], 0);
						al_draw_scaled_bitmap(BMP_MUSIC, 0, 0, al_get_bitmap_width(BMP_MUSIC), al_get_bitmap_height(BMP_MUSIC), constants_to_set_graphics[2][0], constants_to_set_graphics[1][0], al_get_bitmap_width(BMP_MUSIC)*scale[0], al_get_bitmap_height(BMP_MUSIC)*scale[1], 0);
						al_draw_scaled_bitmap(BMP_MUSIC_CURRENT, 0, 0, al_get_bitmap_width(BMP_MUSIC_CURRENT), al_get_bitmap_height(BMP_MUSIC_CURRENT), constants_to_set_graphics[2][1], constants_to_set_graphics[1][0], al_get_bitmap_width(BMP_MUSIC_CURRENT)*scale[0], al_get_bitmap_height(BMP_MUSIC_CURRENT)*scale[1], 0);
						if (!BMP_START || !BMP_MUSIC || !BMP_MUSIC_CURRENT) {
							int error;
							printf("Music menu didnt load correctly. Program will exit\nEnter any key to continue");
							scanf_s("%i", &error);
							exit(EXIT_FAILURE);
						}
						music_on = false;
						al_stop_samples();
						al_flip_display();
					}
				}
				break;

			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = true;
				if (remember == 2) {
					if (!music_on) {
						BMP_START = al_load_bitmap("wejsciowe/tlo_podstawowe2.png");
						BMP_MUSIC = al_load_bitmap("wejsciowe/MUZYKA.png");
						BMP_MUSIC_CURRENT = al_load_bitmap("wejsciowe/WLACZONA.png");
						if (!BMP_START || !BMP_MUSIC || !BMP_MUSIC_CURRENT) {
							int error;
							printf("Music menu didnt load correctly. Program will exit\nEnter any key to continue");
							scanf_s("%i", &error);
							exit(EXIT_FAILURE);
						}
						al_draw_scaled_bitmap(BMP_START, 0, 0, al_get_bitmap_width(BMP_START), al_get_bitmap_height(BMP_START), 0, 0, al_get_bitmap_width(BMP_START)*scale[0], al_get_bitmap_height(BMP_START)*scale[1], 0);
						al_draw_scaled_bitmap(BMP_MUSIC, 0, 0, al_get_bitmap_width(BMP_MUSIC), al_get_bitmap_height(BMP_MUSIC), constants_to_set_graphics[2][0], constants_to_set_graphics[1][0], al_get_bitmap_width(BMP_MUSIC)*scale[0], al_get_bitmap_height(BMP_MUSIC)*scale[1], 0);
						al_draw_scaled_bitmap(BMP_MUSIC_CURRENT, 0, 0, al_get_bitmap_width(BMP_MUSIC_CURRENT), al_get_bitmap_height(BMP_MUSIC_CURRENT), constants_to_set_graphics[2][1], constants_to_set_graphics[1][0], al_get_bitmap_width(BMP_MUSIC_CURRENT)*scale[0], al_get_bitmap_height(BMP_MUSIC_CURRENT)*scale[1], 0);
						al_flip_display();
						al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
						music_on = true;
					}
				}
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_ESCAPE:
				count_enter--;
				if (remember == 0 || remember == 2) remember = -10;
				if (count_enter == -1) {
					left = true;
				}
				if (count_enter == 0) {
					al_draw_bitmap(BMP_START, 0, 0, 0);
					locked[0] = false;
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
				count_enter++;
				switch (menu[0]) {
				case 0:
					if (!locked[0]) {
						is_back = false;
						locked[0] = true;
					}
					al_draw_scaled_bitmap(BMP_START, 0, 0, al_get_bitmap_width(BMP_START), al_get_bitmap_height(BMP_START), 0, 0, al_get_bitmap_width(BMP_START)*scale[0], al_get_bitmap_height(BMP_START)*scale[1], 0);
					remember = 0;
					if (count_enter > 1) {
						switch (menu[1]) {
						case 0: number_of_players[0] = true; number_of_players[1] = false; left_menu = true; in_game = true; al_clear_to_color(al_map_rgb(0, 0, 0)); pre_start_game(); break;
						case 1:  number_of_players[0] = true; number_of_players[1] = true; left_menu = true; in_game = true; al_clear_to_color(al_map_rgb(0, 0, 0));  pre_start_game(); break;
						}
						count_enter--;
					}
					if (count_enter_choice > 1) {
						left_menu = true;
					}
					break;
				case 1: count_enter--; break;
				case 2:
					remember = 2;
					if (!locked[0]) {
						is_back = false;
						locked[0] = true;
					}
					al_draw_scaled_bitmap(BMP_START, 0, 0, al_get_bitmap_width(BMP_START), al_get_bitmap_height(BMP_START), 0, 0, al_get_bitmap_width(BMP_START)*scale[0], al_get_bitmap_height(BMP_START)*scale[1], 0);
					al_draw_scaled_bitmap(BMP_MUSIC, 0, 0, al_get_bitmap_width(BMP_MUSIC), al_get_bitmap_height(BMP_MUSIC), constants_to_set_graphics[2][0], constants_to_set_graphics[1][0], al_get_bitmap_width(BMP_MUSIC)*scale[0], al_get_bitmap_height(BMP_MUSIC)*scale[1], 0);
					al_draw_scaled_bitmap(BMP_MUSIC_CURRENT, 0, 0, al_get_bitmap_width(BMP_MUSIC_CURRENT), al_get_bitmap_height(BMP_MUSIC_CURRENT), constants_to_set_graphics[2][1], constants_to_set_graphics[1][0], al_get_bitmap_width(BMP_MUSIC_CURRENT)*scale[0], al_get_bitmap_height(BMP_MUSIC_CURRENT)*scale[1], 0);
					if (++count_enter > 1) count_enter = 1;
					break;

				case 3:  count_enter--;  break;
				case 4: left = true; break;
				}
				al_flip_display();
			}
		}
		else if (ev.type == ALLEGRO_EVENT_TIMER) {
			if (keys[DOWN] == false && !locked[0] && !left_menu) {
				switch (menu[0]) {
				case 0:  BMP_NEWGAME = al_load_bitmap("wejsciowe/NOWA_GRA_WYBRANE.PNG"); BMP_RECORDS = al_load_bitmap("wejsciowe/REKORDY.png"); break;
				case 1:  BMP_RECORDS = al_load_bitmap("wejsciowe/REKORDY_WYBRANE.png"); BMP_NEWGAME = al_load_bitmap("wejsciowe/NOWA_GRA.png"); BMP_OPTIONS = al_load_bitmap("wejsciowe/OPCJE.png"); break;
				case 2:  BMP_OPTIONS = al_load_bitmap("wejsciowe/OPCJE_WYBRANE.png"); BMP_RECORDS = al_load_bitmap("wejsciowe/REKORDY.png"); BMP_CREDITS = al_load_bitmap("wejsciowe/CREDITS.png"); break;
				case 3:  BMP_CREDITS = al_load_bitmap("wejsciowe/CREDITS_WYBRANE.png"); BMP_OPTIONS = al_load_bitmap("wejsciowe/OPCJE.png"); BMP_EXIT = al_load_bitmap("wejsciowe/ZAKONCZ_GRE.png"); break;
				case 4:  BMP_EXIT = al_load_bitmap("wejsciowe/ZAKONCZ_GRE_WYBRANE.png"); BMP_CREDITS = al_load_bitmap("wejsciowe/CREDITS.png"); break;
				}
				al_draw_scaled_bitmap(BMP_START, 0, 0, al_get_bitmap_width(BMP_START), al_get_bitmap_height(BMP_START), 0, 0, al_get_bitmap_width(BMP_START)*scale[0], al_get_bitmap_height(BMP_START)*scale[1], 0);
				al_draw_scaled_bitmap(BMP_NEWGAME, 0, 0, al_get_bitmap_width(BMP_NEWGAME), al_get_bitmap_height(BMP_NEWGAME),constants_to_set_graphics[0][0], constants_to_set_graphics[0][1] + constants_to_set_graphics[0][2], al_get_bitmap_width(BMP_NEWGAME)*scale[0], al_get_bitmap_height(BMP_NEWGAME)*scale[1], 0);
				al_draw_scaled_bitmap(BMP_RECORDS, 0, 0, al_get_bitmap_width(BMP_RECORDS), al_get_bitmap_height(BMP_RECORDS), constants_to_set_graphics[0][0], constants_to_set_graphics[0][1] + 2*constants_to_set_graphics[0][2], al_get_bitmap_width(BMP_RECORDS)*scale[0], al_get_bitmap_height(BMP_RECORDS)*scale[1], 0);
				al_draw_scaled_bitmap(BMP_OPTIONS, 0, 0, al_get_bitmap_width(BMP_OPTIONS), al_get_bitmap_height(BMP_OPTIONS), constants_to_set_graphics[0][0], constants_to_set_graphics[0][1] + 3*constants_to_set_graphics[0][2], al_get_bitmap_width(BMP_OPTIONS)*scale[0], al_get_bitmap_height(BMP_OPTIONS)*scale[1], 0);
				al_draw_scaled_bitmap(BMP_CREDITS, 0, 0, al_get_bitmap_width(BMP_CREDITS), al_get_bitmap_height(BMP_CREDITS), constants_to_set_graphics[0][0], constants_to_set_graphics[0][1] + 4*constants_to_set_graphics[0][2], al_get_bitmap_width(BMP_CREDITS)*scale[0], al_get_bitmap_height(BMP_CREDITS)*scale[1], 0);
				al_draw_scaled_bitmap(BMP_EXIT, 0, 0, al_get_bitmap_width(BMP_EXIT), al_get_bitmap_height(BMP_EXIT), constants_to_set_graphics[0][0], constants_to_set_graphics[0][1] + 5*constants_to_set_graphics[0][2], al_get_bitmap_width(BMP_EXIT)*scale[0], al_get_bitmap_height(BMP_EXIT)*scale[1], 0);
			}
			if (keys[DOWN] == false && locked[0] && remember == 0 && !left_menu) {
				switch (menu[1]) {
				case 0:  BMP_ONE_PLAYER = al_load_bitmap("wejsciowe/JEDEN_GRACZ_WYBRANE.PNG"); BMP_TWO_PLAYERS = al_load_bitmap("wejsciowe/DWOCH_GRACZY.png"); break;
				case 1:  BMP_ONE_PLAYER = al_load_bitmap("wejsciowe/JEDEN_GRACZ.PNG"); BMP_TWO_PLAYERS = al_load_bitmap("wejsciowe/DWOCH_GRACZY_WYBRANE.png");  break;
				}
				al_draw_scaled_bitmap(BMP_ONE_PLAYER, 0, 0, al_get_bitmap_width(BMP_ONE_PLAYER), al_get_bitmap_height(BMP_ONE_PLAYER), constants_to_set_graphics[1][0], constants_to_set_graphics[0][1] + constants_to_set_graphics[0][2], al_get_bitmap_width(BMP_ONE_PLAYER)*scale[0], al_get_bitmap_height(BMP_ONE_PLAYER)*scale[1], 0);
				al_draw_scaled_bitmap(BMP_TWO_PLAYERS, 0, 0, al_get_bitmap_width(BMP_TWO_PLAYERS), al_get_bitmap_height(BMP_TWO_PLAYERS), constants_to_set_graphics[1][0], constants_to_set_graphics[0][1] + constants_to_set_graphics[0][2]*2, al_get_bitmap_width(BMP_TWO_PLAYERS)*scale[0], al_get_bitmap_height(BMP_TWO_PLAYERS)*scale[1], 0);
			}
			if (keys[DOWN] == false && locked[0] && remember == 2 && !left_menu) {

			}
			al_flip_display();
		}
	}
	al_destroy_bitmap(BMP_START);
	al_destroy_bitmap(BMP_NEWGAME);
	al_destroy_bitmap(BMP_CREDITS);
	al_destroy_bitmap(BMP_EXIT);
	al_destroy_bitmap(BMP_OPTIONS);
	al_destroy_bitmap(BMP_RECORDS);
	al_destroy_bitmap(BMP_ONE_PLAYER);
	al_destroy_bitmap(BMP_TWO_PLAYERS);
	al_destroy_bitmap(BMP_MUSIC);
	al_destroy_bitmap(BMP_MUSIC_CURRENT);
	al_destroy_bitmap(BMP_CHARACTER_UP);
	al_destroy_bitmap(BMP_CHARACTER_DOWN);
	al_destroy_bitmap(BMP_CHARACTER_LEFT);
	al_destroy_bitmap(BMP_CHARACTER_RIGHT);
	al_destroy_bitmap(BMP_CHARACTER_UP_2);
	al_destroy_bitmap(BMP_CHARACTER_DOWN_2);
	al_destroy_bitmap(BMP_CHARACTER_LEFT_2);
	al_destroy_bitmap(BMP_CHARACTER_RIGHT_2);
	al_destroy_bitmap(BMP_TEXTURE_1);
	al_destroy_bitmap(BMP_TEXTURE_2);
	al_destroy_bitmap(BMP_TEXTURE_3);
	al_destroy_bitmap(BMP_TEXTURE_4);
	al_destroy_bitmap(BMP_TEXTURE_5);
	al_destroy_bitmap(BMP_SEMESTER_OVER);
	al_destroy_bitmap(BMP_BULLET);
	al_destroy_bitmap(BMP_BULLET_2);
	al_destroy_bitmap(BMP_ENEMY);
	al_destroy_bitmap(BMP_ENEMY_BOSS);
	al_destroy_bitmap(BMP_BULLET_ENEMY);
	al_destroy_bitmap(BMP_SYMBOL);
	al_destroy_bitmap(icon);
	al_destroy_bitmap(BMP_FREEZE);
	al_destroy_bitmap(BMP_UNDEAD);
	al_destroy_bitmap(BMP_LIFE);
	al_destroy_bitmap(BMP_RENOVATION);
	al_destroy_bitmap(BMP_DESTROY);
	al_destroy_sample(sample);
	al_destroy_timer(timer);
	al_destroy_event_queue(event_queue);
	al_destroy_display(display);

}