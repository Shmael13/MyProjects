#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <math.h>

int NUMBALLS = 10;
float FORCE = 100.1f; // strength of attraction/repulsion

void must_init(bool test, const char* description)
{
	if (test) return;
	printf("coudn't initialize %s\n", description);
	exit(1);
}

enum BALL_TYPE {
	BT_BLUE = 0,
	BT_YELLOW,
	BT_N
};

typedef struct BALL{
	float x, y;
	float dx, dy;
	int type;
}BALL;

float distance(BALL* a, BALL* b) {
    float dx = a->x - b->x;
    float dy = a->y - b->y;
    return sqrt(dx * dx + dy * dy);
}

void init_balls(BALL obj[]){

	for(int i = 0; i < BT_N * NUMBALLS; i++){
		BALL* b = &obj[i];
		b->x = rand() % 640;
		b->y = rand() % 480;
		b->dx = 1; // ((((float)rand()) / RAND_MAX) -0.5) * 2 * 4;
		b->dy = 0; // ((((float)rand()) / RAND_MAX) -0.5) * 2 * 4;
		b->type = i % BT_N;
    if (b->type == BT_BLUE){
      b->x = 426;
      b->dx = -1;
      b->dy = 0.00001f;
    }
	}
}

void redraw_balls(BALL obj[]){
	al_clear_to_color(al_map_rgb(0,0,0));
	for (int i = 0; i < BT_N * NUMBALLS; i++){
		BALL* b = &obj[i];
		ALLEGRO_COLOR color;
		if (b->type == BT_BLUE){color = al_map_rgb_f(0, 190, 255);}
		if (b->type == BT_YELLOW){color = al_map_rgb_f(255, 255, 0);}
		al_draw_circle(b->x, b->y, 1, color, 10);
	}
	al_flip_display();
}

void update_ball_position(BALL obj[]){
      for (int i = 0; i < BT_N * NUMBALLS; i++){
	    BALL* b = &obj[i];
	    b->x += b->dx;
	    b->y += b->dy;
	    if(b->x < 0){
		b->x *= -1;
		b->dx *= -0.5;
	    }
	    if (b->x > 640){
		b->x = 2*640 - b->x;
		b->dx *= -0.5;
	    }
	    if (b->y < 0){
		b->y *= -1;
		b->dy *= -0.5;
	    }
	    if(b->y > 480){
		b->y = 2*480 - b->y;
		b->dy *= -0.5;
	    }

      }
}
void update_ball_velocity(BALL obj[]){			    
	// Attraction/Repulsion logic
	
  for (int i = 0; i < BT_N * NUMBALLS; i++){
    for (int j = 0; j < BT_N * NUMBALLS; j++){
      if (i != j){ //&& (obj[i].type != obj[j].type)){
        float dist = distance(&obj[i], &obj[j]);
        if (dist > 0){
          float dx = obj[j].x - obj[i].x; //x component
          float dy = obj[j].y - obj[i].y; //y component
          float gravity = FORCE / (dist * dist);
          dx /= dist;
          dy /= dist;
          obj[i].dx += dx * gravity;
          obj[i].dy += dy * gravity;
        }
      }
    }
  }

  //float dx = obj[1].x - obj[0].x;
	//float dy = obj[1].y - obj[0].y;
	//float distances[BT_N][NUMBALLS];
	//for (int i = 0; i < BT_N; i++){
	//	for (int j=0; j < NUMBALLS; j++){
	//		distances[i][j] = distance(&obj[i], &obj[j]);
	//	}
	//}
	//
	//for (int i = 0; i < BT_N * NUMBALLS; i++){
	//	for (int j = 0; j < BT_N * NUMBALLS; j++){
	//		if (j % BT_N != 0){//Balls of the same color have no effect on each other	
	//			float dist = distances[i][j];
	//			if (dist > 0) {
	//				dx /= dist;
	//				dy /= dist;
	//			}
	//			obj[i].dx -= dx * FORCE;
	//			obj[i].dy -= dy * FORCE;
	//		}
	//	}
	//	// Yellow (index 1) attracted to Blue (index 0)
	//}
}
				
int main(){
	must_init(al_init(), "allegro");
	must_init(al_install_keyboard(), "keyboard");
	
	ALLEGRO_TIMER* timer = al_create_timer(1.0/60.0);
	must_init(timer, "timer");

	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
	must_init(queue, "event queue");

	al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
	al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
	al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

	ALLEGRO_DISPLAY* disp = al_create_display(640, 480);
	must_init(disp, "display");

  	must_init(al_init_primitives_addon(), "primitives");

	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(disp));
	al_register_event_source(queue, al_get_timer_event_source(timer));

	bool done = false;
	bool redraw = true;
	ALLEGRO_EVENT event;
	BALL obj[BT_N * NUMBALLS];
	
	init_balls(obj);
	al_start_timer(timer);
	while(1){
		al_wait_for_event(queue, &event);
		switch(event.type){
			case ALLEGRO_EVENT_TIMER:
			    	update_ball_velocity(obj);
				update_ball_position(obj);
                        	redraw = true;
                        	break;
                         
			case ALLEGRO_EVENT_KEY_DOWN:
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				done = true;
				break;
				
		}	
		if (done){
			break;
		}
		if (redraw && al_is_event_queue_empty(queue)){
			redraw_balls(obj);
			redraw = false;
		}

	}
	al_destroy_timer(timer);
	al_destroy_event_queue(queue);
	al_destroy_display(disp);

	return 0;
}
