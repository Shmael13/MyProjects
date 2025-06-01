#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>

#define WINDOW_WIDTH 700
#define WINDOW_HEIGHT 700

#define BALL_RADIUS 5
#define NUM_TYPES 3
#define BALLS_PER_TYPE 130
#define TOTAL_BALLS (NUM_TYPES * BALLS_PER_TYPE)

#define GRAVITY 10.0f
#define REPULSION 50.0f
#define MIN_DIST 1.0f
#define FRICTION 0.9f
#define MAX_SPEED 5.0f

#define MIN_COL_DIST 10
#define MAX_COL_DIST 60
#define MIN_FORCE -1
#define MAX_FORCE 1

#define RAND_FORCES 1
#define RAND_DIST 1

typedef struct {
    float x, y;
    float dx, dy;
    int type;
} Ball;

enum {
    TYPE_BLUE = 0,
    TYPE_YELLOW,
    TYPE_GREEN
};

int color_distances[NUM_TYPES + 1][NUM_TYPES + 1] = {{60, 60, 100}, {100, 100, 100}, {60, 60, 60}};
float color_forces[NUM_TYPES + 1][NUM_TYPES + 1] = {{1, 1, -0.5}, {0.5, 0.5, 0.25}, {1, 0.75, 0.5}};

float rand_float(float min, float max) {
    return min + ((float)rand() / (float)RAND_MAX) * (max - min);
}

int getRandomInt(int min, int max) {
    return rand() % (max - min + 1) + min;
}

void randomize_color_dist(void){
  for (int i = 0; i == NUM_TYPES; i++){
    for (int j = 0; j == NUM_TYPES; j++){
      color_distances[i][j] = getRandomInt(MIN_COL_DIST, MAX_COL_DIST);
    }
  }
}

void randomize_color_forces(void){
  for (int i = 0; i <= NUM_TYPES; i++){
    for (int j = 0; j <= NUM_TYPES; j++){
      color_forces[i][j] = rand_float(MIN_FORCE, MAX_FORCE);
    }
  }
}

void must_init(bool test, const char *description) {
    if (!test) {
        printf("Failed to initialize: %s\n", description);
        exit(1);
    }
}

float distance_squared(float dx, float dy) {
    return dx * dx + dy * dy;
}

float clamp_speed(float v, float max) {
    return (v > max) ? max : ((v < -max) ? -max : v);
}

// Returns the force magnitude between two types at a distance
float compute_force(int type1, int type2, float dist) {
    // Distance-based custom interaction logic
    float col_forces = color_forces[type1][type2];
    int col_dist = color_distances[type1][type2];
    return (dist < col_dist) ? -REPULSION / dist : col_forces;
}

void apply_forces(Ball balls[]) {
    for (int i = 0; i < TOTAL_BALLS; i++) {
        Ball *a = &balls[i];
        for (int j = 0; j < TOTAL_BALLS; j++) {
            if (i == j) continue;
            Ball *b = &balls[j];

            float dx = b->x - a->x;
            float dy = b->y - a->y;
            float dist2 = distance_squared(dx, dy);
            if (dist2 < 1) continue; // avoid div by 0
            float dist = sqrtf(dist2);

            float force = compute_force(a->type, b->type, dist);
            dx /= dist;
            dy /= dist;

            a->dx += dx * force;
            a->dy += dy * force;
        }

        // Apply friction and clamp velocity
        a->dx *= FRICTION;
        a->dy *= FRICTION;
        a->dx = clamp_speed(a->dx, MAX_SPEED);
        a->dy = clamp_speed(a->dy, MAX_SPEED);
    }
}

void update_positions(Ball balls[]) {
    for (int i = 0; i < TOTAL_BALLS; i++) {
        Ball *b = &balls[i];
        b->x += b->dx;
        b->y += b->dy;
        
        if (b->y >= WINDOW_HEIGHT){
          b->y -= WINDOW_HEIGHT;
        }
        else if (b->y <= 0){
          b->y += WINDOW_HEIGHT;
        }

        if (b->x >= WINDOW_WIDTH){
          b->x -= WINDOW_WIDTH;
        }
        else if (b->x <= 0){
          b->x += WINDOW_WIDTH;
        }
        
        // Bounce off walls
        //if (b->x < 0 || b->x > WINDOW_WIDTH) {
        //    b->dx *= -1;
        //    b->x = fminf(fmaxf(b->x, 0), WINDOW_WIDTH);
        //}
        //if (b->y < 0 || b->y > WINDOW_HEIGHT) {
        //    b->dy *= -1;
        //    b->y = fminf(fmaxf(b->y, 0), WINDOW_HEIGHT);
        //}
    }
}

void draw_balls(Ball balls[]) {
    al_clear_to_color(al_map_rgb(0, 0, 0));
    for (int i = 0; i < TOTAL_BALLS; i++) {
        ALLEGRO_COLOR color;
        switch (balls[i].type) {
            case TYPE_BLUE: color = al_map_rgb(0, 150, 255); break;
            case TYPE_YELLOW: color = al_map_rgb(255, 255, 0); break;
            case TYPE_GREEN: color = al_map_rgb(0, 255, 100); break;
        }
        al_draw_filled_circle(balls[i].x, balls[i].y, BALL_RADIUS, color);
    }
    al_flip_display();
}

void init_balls(Ball balls[]) {
    for (int t = 0; t < NUM_TYPES; t++) {
        for (int i = 0; i < BALLS_PER_TYPE; i++) {
            int idx = t * BALLS_PER_TYPE + i;
            balls[idx].x = rand() % WINDOW_WIDTH;
            balls[idx].y = rand() % WINDOW_HEIGHT;
            balls[idx].dx = (((float)rand() / RAND_MAX) - 0.5f) * 2;
            balls[idx].dy = (((float)rand() / RAND_MAX) - 0.5f) * 2;
            balls[idx].type = t;
        }
    }
}

int main() {
    srand(time(NULL));
    must_init(al_init(), "Allegro");
    must_init(al_install_keyboard(), "Keyboard");
    must_init(al_init_primitives_addon(), "Primitives");
    if (RAND_FORCES){
      randomize_color_forces();
    }
    if (RAND_DIST){
      randomize_color_dist();
    }

    ALLEGRO_DISPLAY *disp = al_create_display(WINDOW_WIDTH, WINDOW_HEIGHT);
    must_init(disp, "Display");

    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60.0);
    must_init(timer, "Timer");

    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    must_init(queue, "Queue");

    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_keyboard_event_source());

    Ball balls[TOTAL_BALLS];
    init_balls(balls);

    bool running = true;
    bool redraw = true;
    ALLEGRO_EVENT ev;
    al_start_timer(timer);

    while (running) {
        al_wait_for_event(queue, &ev);
        switch (ev.type){
          case ALLEGRO_EVENT_TIMER:
            apply_forces(balls);
            update_positions(balls);
            redraw = true;
            break;
          case ALLEGRO_EVENT_KEY_DOWN:
            if (ev.keyboard.keycode == ALLEGRO_KEY_R) {
              randomize_color_forces();
              randomize_color_dist();
              break;
            }
          case ALLEGRO_EVENT_DISPLAY_CLOSE:
            running = false;
            break;
        }
        if (redraw && al_is_event_queue_empty(queue)) {
            draw_balls(balls);
            redraw = false;
        }
    }

    al_destroy_display(disp);
    al_destroy_event_queue(queue);
    al_destroy_timer(timer);

    return 0;
}

