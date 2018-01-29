#include <iostream>
#include <vector>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

using namespace std;

bool wallCollide(int x, int y, int angle, int dir, int size, int map[64][48]);

const float FPS = 60;
const int SCREEN_W = 640;
const int SCREEN_H = 480;
const int player1_SIZE = 32;

enum MYKEYS {
	KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_M
};
enum DIRECTIONS {
	RIGHT, LEFT, UP, DOWN
};


int main() {
	//the map
	int level[64][48];
	//initialize to all 0s
	for (int m = 0; m < 64; m++)
		for (int n = 0; n < 48; n++) {
			//fill in perimeter
			if (m == 0 || n == 0)
				level[m][n] = 1;
			else if (m == 63 || n == 47)
				level[m][n] = 1;
			//draw interior maze walls
			else if (m == 20 && n < 30)
				level[m][n] = 1;
			else if (m > 20 && n == 35)
				level[m][n] = 1;
			else if (m > 35 && n == 15)
				level[m][n] = 1;
			//leave rest of map blank
			else
				level[m][n] = 0;
		}
	//print for TESTING!!
	for (int m = 0; m < 64; m++)
		for (int n = 0; n < 48; n++)
			cout << level[m][n];



	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER * timer = NULL;
	ALLEGRO_BITMAP * player1 = NULL;
	float player1_x = SCREEN_W / 2. - player1_SIZE / 2.0;
	float player1_y = SCREEN_H / 2. - player1_SIZE / 2.0;
	bool key[5] = { false, false, false, false, false };
	bool redraw = true;
	bool doexit = false;
	double angle = 0;
	int justfired = 0;

	al_init();
	al_init_image_addon();
	al_init_primitives_addon();
	al_install_keyboard();
	timer = al_create_timer(1.0 / FPS);
	display = al_create_display(SCREEN_W, SCREEN_H);
	player1 = al_load_bitmap("tank.jpg");

	al_set_target_bitmap(al_get_backbuffer(display));
	event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_flip_display();
	al_start_timer(timer);

	while (!doexit) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);
		
		if (ev.type == ALLEGRO_EVENT_TIMER) {



			if (angle < 0)
				angle = 360;
			if (angle > 360)
				angle = 0;

			if (key[KEY_UP] && !wallCollide(player1_x, player1_y, UP, angle, 32, level)) {

				player1_y += 4 * sin(3.14*angle / 100);
				player1_x += 4 * cos(3.14*angle / 100);
			}
			if (key[KEY_DOWN] && !wallCollide(player1_x, player1_y, DOWN, angle, 32, level)) {

				player1_y -= 4 * sin(3.14*angle / 100);
				player1_x -= 4 * cos(3.14*angle / 100);
			}

			if (key[KEY_LEFT]) {

				angle -= 1;
			}
			if (key[KEY_RIGHT]) {

				angle += 1;
			}

			redraw = true;
		}//end timer section

		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			break;
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {//This
			switch (ev.keyboard.keycode) {//That
			case ALLEGRO_KEY_UP:
				key[KEY_UP] = true;
				break;

			case ALLEGRO_KEY_DOWN:
				key[KEY_DOWN] = true;
				break;

			case ALLEGRO_KEY_LEFT:
				key[KEY_LEFT] = true;
				break;

			case ALLEGRO_KEY_RIGHT:
				key[KEY_RIGHT] = true;
				break;

			case ALLEGRO_KEY_M:
				key[KEY_M] = true;
				break;
			}//That
		}//This
		else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_UP:
				key[KEY_UP] = false;
				break;

			case ALLEGRO_KEY_DOWN:
				key[KEY_DOWN] = false;
				break;

			case ALLEGRO_KEY_LEFT:
				key[KEY_LEFT] = false;
				break;

			case ALLEGRO_KEY_RIGHT:
				key[KEY_RIGHT] = false;
				break;

			case ALLEGRO_KEY_ESCAPE:
					doexit = true;
					break;
			case ALLEGRO_KEY_M:
				key[KEY_M] = false;
				break;
			}
		}
		
		if (redraw && al_is_event_queue_empty(event_queue)) {
			redraw = false;
			al_clear_to_color(al_map_rgb(0, 0, 0));

			for (int m = 0; m < 64; m++)
				for (int n = 0; m < 46; n++)
					if (level[m][n] == 1)
						al_draw_filled_rectangle(m * 10, n * 10, m * 10 + 10, n * 10 + 10, al_map_rgb(255, 25, 255));

			al_draw_roated_bitmap(player1, 16, 16, player1_x, player1_y, ((angle*3.14 / 100)), 0);
		}


	}//end loop


}