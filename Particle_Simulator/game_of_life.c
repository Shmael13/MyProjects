#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <math.h>

int NUMBALLS = 9;
float GRAVITY = 10.0f; // strength of attraction/repulsion
float FRICTION = 0.1f;
float REPULSION_STRENGTH = 5.0f;
int BALL_SIZE = 10;
float SIM_SPEED = 60.0;
float WALL_REPULSION = 1.5f;

int WINDOW_LEN = 1000;
int WINDOW_WIDTH = 700;

void must_init(bool test, const char* description)
{
	if (test) return;
	printf("coudn't initialize %s\n", description);
	exit(1);
}

enum BALL_TYPE {
	BT_BLUE = 0,
	BT_YELLOW,
 	BT_GREEN, 
	BT_N
};

typedef struct BALL{
	float x, y;
	float dx, dy;
  float radius;
	int type;
}BALL;

// The order is in order of appearance in BT_TYPE
// so, blue to blue, blue to yellow, yellow to blue, yellow to yellow
float color_repvals[BT_N][BT_N] = {{0.3, -0.5, -0.5}, {0.6, -0.6, 1}, {0.35, 0.6, -1}};


float distance(BALL* a, BALL* b) {
    float dx = a->x - b->x;
    float dy = a->y - b->y;
    return sqrt(dx * dx + dy * dy);
}

void init_balls(BALL obj[]){

	for(int i = 0; i < BT_N * NUMBALLS; i++){
		BALL* b = &obj[i];
		b->x = rand() % WINDOW_LEN;
		b->y = rand() % WINDOW_WIDTH;
		b->dx = 1; // ((((float)rand()) / RAND_MAX) -0.5) * 2 * 4;
		b->dy = 0; // ((((float)rand()) / RAND_MAX) -0.5) * 2 * 4;
		b->type = i % BT_N;
    b->radius = BALL_SIZE;
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
    		if (b->type == BT_GREEN){color = al_map_rgb_f(0, 255, 0);}
		al_draw_circle(b->x, b->y, b->radius, color, 20);
	}
	al_flip_display();
}

void apply_friction(BALL* obj){
  obj->dx *= (1 - FRICTION);
  obj->dy *= (1 - FRICTION);
}

void update_ball_position(BALL obj[]){
  for (int i = 0; i < BT_N * NUMBALLS; i++){
    BALL* b = &obj[i];
    b->x += b->dx;
    b->y += b->dy;

    if(b->x < 0){
      b->x *= -WALL_REPULSION;
      b->dx *= -WALL_REPULSION;
    }

    if (b->x > WINDOW_LEN){
      b->x = 2*WINDOW_LEN - b->x;
      b->dx *= -WALL_REPULSION;
    }

    if (b->y < 0){
      b->y *= -WALL_REPULSION;
      b->dy *= -WALL_REPULSION;
    }

    if(b->y > WINDOW_WIDTH){
      b->y = 2*WINDOW_WIDTH - b->y;
      b->dy *= -WALL_REPULSION;
    }

  }
}
void update_ball_velocity(BALL obj[]){			    
	// Attraction/Repulsion logic
	
  for (int i = 0; i < BT_N * NUMBALLS; i++){
    for (int j = 0; j < BT_N * NUMBALLS; j++){
      if (i != j){
        //float dist = distance(&obj[i], &obj[j]);
        float dx = obj[j].x - obj[i].x;
        float dy = obj[j].y - obj[i].y;
        float dist_squared = dx * dx + dy * dy;
        float min_dist = obj[i].radius + obj[j].radius;
        float min_dist_squared = min_dist * min_dist;
        float dist = sqrt(dist_squared);
        dx /= dist;
        dy /= dist;

        float force = 0.0f;
        
        if (dist_squared > 100 * min_dist_squared) {
          force = 0.0f;
        }
        else if (dist_squared < min_dist_squared) {
          // Repulsion if too close
          force = -REPULSION_STRENGTH * (1.0f - dist_squared / min_dist_squared);
        } 
        else {
          // Attraction with gravity-like force
          force = GRAVITY / dist_squared;
          force *= color_repvals[obj[i].type][obj[j].type];
        }
        
       //if (dist > (3 * obj->radius * obj->radius)){ //very far, no effect
       //   force = 0;
       // }
       // else if (dist < (obj->radius * obj->radius)){     //too close, push away
       //   force = -(1.0 / exp(dist * obj->radius));
       // }        
       // 
       // else if (dist > 0){ //normal case
       //   force = GRAVITY / (dist * dist);
       // }
       // 
       // dx /= dist;
       // dy /= dist;
        obj[i].dx += dx * force;
        obj[i].dy += dy * force;
      }
    }
    apply_friction(&obj[i]);
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
	//			obj[i].dx -= dx * GRAVITY;
	//			obj[i].dy -= dy * GRAVITY;
	//		}
	//	}
	//	// Yellow (index 1) attracted to Blue (index 0)
	//}
				
int main(){
	must_init(al_init(), "allegro");
	must_init(al_install_keyboard(), "keyboard");
	
	ALLEGRO_TIMER* timer = al_create_timer(1.0/60.0);
	must_init(timer, "timer");

	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
	must_init(queue, "event queue");
  
  //Increase the quality
	al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
	al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
	al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

	ALLEGRO_DISPLAY* disp = al_create_display(WINDOW_LEN, WINDOW_WIDTH);
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
