//clusters.c
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <signal.h>
#include <pthread.h>
#include <omp.h>
#include "cluster_utils.h"

#define WINDOW_WIDTH 700
#define WINDOW_HEIGHT 700
#define FRAME_RATE 20

#define GRID_SIZE 25 //MUST BE BIGGER THAN MAX_COL_DIST(in cluster_utils.h)
#define GRID_COLS (WINDOW_WIDTH / GRID_SIZE)
#define GRID_ROWS (WINDOW_HEIGHT / GRID_SIZE)
#define GRID_CELLCOUNT (GRID_COLS * GRID_ROWS)

#define BALL_RADIUS 1.5
#define BALLS_PER_TYPE 300
#define TOTAL_BALLS (NUM_TYPES * BALLS_PER_TYPE)

#define REPULSION 5.0f

//Higher number is less friction, FRICTION of 1 is equivalent to no friction 
#define FRICTION 0.8f
#define MAX_SPEED 40.0f

#define RAND_FORCES true
#define RAND_DIST false

#define THREAD_THRESHOLD (BALLS_PER_TYPE / 2) //If this many particles are present within a single cell, it spawns a new thread to speedup calculation

enum {
    TYPE_BLUE = 0,
    TYPE_YELLOW,
    TYPE_GREEN,
    TYPE_PURPLE,
    TYPE_RED,
    TYPE_CYAN,
    TYPE_ORANGE,
    TYPE_PINK
};

Ball* grid_buffer[TOTAL_BALLS];

//Each grid cell contains grid_buffer start index, and count of balls in the cell
typedef struct{
  int start;
  int count;
} CellIndex;

typedef struct {
    int tid;
    const CellIndex* effected_cip;
    const CellIndex* appl_cip;
} CellWorkerArgs;


CellIndex cell_Indicies[GRID_CELLCOUNT];

void clear_grid(void){
  for (int i = 0; i < GRID_CELLCOUNT; i++){
    cell_Indicies[i].count = 0;
    cell_Indicies[i].start = 0;
  }
}

void assign_balls_to_grid(Ball balls[]){
  clear_grid();
  int ball_idx[TOTAL_BALLS];
  //Count how many balls belong in each cell
  for (int i = 0; i < TOTAL_BALLS; i++){
    Ball* b = &balls[i];
    int grid_col = ((int)(b->x / GRID_SIZE) + GRID_COLS) % GRID_COLS;
    int grid_row = ((int)(b->y / GRID_SIZE) + GRID_ROWS) % GRID_ROWS;

    //int grid_row = (int)b->y / GRID_SIZE;
    //int grid_col = (int)b->x / GRID_SIZE;
    if (grid_col < 0 || grid_col >= GRID_COLS || grid_row < 0 || grid_row >= GRID_ROWS) {
      printf("WARNING: Ball out of grid bounds: (%.1f, %.1f) ", b->x, b->y);
      printf("%d, dx: %f, dy: %f\n", b->type, b->dx, b->dy);
    }
    int index = grid_row * GRID_COLS + grid_col;
    ball_idx[i] = index;
    cell_Indicies[index].count++;
  }

  //Assign the start index.
  int start = 0;
  for (int i = 0; i < GRID_CELLCOUNT; i++){
    cell_Indicies[i].start = start;
    start += cell_Indicies[i].count;
    cell_Indicies[i].count = 0;  //Clear the count - to be used in the next step
  }

  //Fill the grid_buffer
  for (int i = 0; i < TOTAL_BALLS; i++){
    Ball* b = &balls[i];
    int index = ball_idx[i];
    int insert_idx = cell_Indicies[index].start + cell_Indicies[index].count;
    grid_buffer[insert_idx] = b;
    cell_Indicies[index].count++;
  }
}


// Returns the force magnitude between two types at a distance
float compute_force(int type1, int type2, float dist) {
    // Distance-based custom interaction logic
    const float max_influence = color_distances[type1][type2];
    if (dist > max_influence){return 0;}

    // Strong repulsion when too close - like a soft "wall"
    if (dist < MIN_COL_DIST) {
        // Linearly increase repulsion as particles overlap more
        float overlap = (MIN_COL_DIST - dist) / MIN_COL_DIST ;  // from 0 to 1
        return -.0f * overlap;  // smooth repulsion, adjust -50.0f to tune push strength
    }
    //the midpoint between the max_ifluence and 0 distance is the maximum force.
    const float ideal = max_influence / 2; 
    const float strength = color_forces[type1][type2];

    // Gaussian parameters
    const float sigma = ideal / 3.0f;
    //Forces follow gaussian distribution
    const float exponent = -((dist - ideal) * (dist - ideal)) / (2 * sigma * sigma);
    
    return strength * expf(exponent);
}

void apply_ball_forces(Ball* effected, Ball* applier){
  float dx = applier->x - effected->x;
  float dy = applier->y - effected->y;
  
  //Wraparound logic for distances 
  if (dx > WINDOW_WIDTH / 2) dx -= WINDOW_WIDTH;
  else if (dx < -WINDOW_WIDTH / 2) dx += WINDOW_WIDTH;
  if (dy > WINDOW_HEIGHT / 2) dy -= WINDOW_HEIGHT;
  else if (dy < -WINDOW_HEIGHT / 2) dy += WINDOW_HEIGHT;
          
  float dist_sq = distance_squared(dx, dy);
  if ((dist_sq < 1e-6f)) return; // avoid div by 0

  float max_dist = color_distances[effected->type][applier->type];
  if (dist_sq > max_dist * max_dist){return;}
  
  float dist = sqrtf(dist_sq);
  if (isnan(dist) || dist <=0.0) return;

  if (dist < 2 * BALL_RADIUS) {
    float overlap = (2 * BALL_RADIUS - dist) / 2.0f;

    // Normalize direction vector
    float push_dx = dx * (overlap / dist);
    float push_dy = dy * (overlap / dist);

    // Move them apart (only affecting the effected ball here)
    effected->x -= push_dx;
    effected->y -= push_dy;


    // Wraparound correction (if needed)
    if (effected->x < 0) effected->x += WINDOW_WIDTH;
    else if (effected->x >= WINDOW_WIDTH) effected->x -= WINDOW_WIDTH;

    if (effected->y < 0) effected->y += WINDOW_HEIGHT;
    else if (effected->y >= WINDOW_HEIGHT) effected->y -= WINDOW_HEIGHT;
}

  float force = compute_force(effected->type, applier->type, dist);
  float inv_dist = 1 / dist;
  dx *= inv_dist;
  dy *= inv_dist;

  effected->dx += dx * force;
  effected->dy += dy * force;
}

void apply_ball_resistances(Ball* this){
  // Apply friction and clamp velocity
  this->dx *= FRICTION;
  this->dy *= FRICTION;
  
  const float speed_sq = this->dx * this->dx + this->dy * this->dy;
  if (speed_sq > MAX_SPEED * MAX_SPEED) {
    float speed = sqrtf(speed_sq);
    this->dx = (this->dx / speed) * MAX_SPEED;
    this->dy = (this->dy / speed) * MAX_SPEED;
  }
}



void apply_cell_forces(int cellnum){
  //each cell interacts with 8 other cells, and itself.
  int row = cellnum / GRID_COLS;
  int col = cellnum % GRID_COLS;

  int interacting_cells[9];
  int count = 0;
  for (int dr = -1; dr <= 1; dr++) {
    for (int dc = -1; dc <= 1; dc++) {
        int n_row = (row + dr + GRID_ROWS) % GRID_ROWS;
        int n_col = (col + dc + GRID_COLS) % GRID_COLS;
        interacting_cells[count++] = n_row * GRID_COLS + n_col;
    }
  }
  
  CellIndex effected_ci = cell_Indicies[cellnum];
  //grid_buffer contains Ball*
  //interacting_cells contains the indexes of the interacting cells within cell_Indicies
  for (int i = 0; i < 9; i++){
    int applier_idx = interacting_cells[i];
    CellIndex appl_ci = cell_Indicies[applier_idx];
    if (effected_ci.count > THREAD_THRESHOLD){
      //apply_parallel_cell_forces(&effected_ci, &appl_ci);
      #pragma omp parallel for schedule(dynamic)
      for (int k = 0; k < effected_ci.count; k++){
        Ball* effected = grid_buffer[effected_ci.start + k];
        
        for (int j = 0; j < appl_ci.count; j++){
          Ball* applier = grid_buffer[appl_ci.start + j];
          if (applier_idx == cellnum && j == k) continue;   //It is the same ball, so no forces act on it
          apply_ball_forces(effected, applier);
        }
        apply_ball_resistances(effected);
      }

    }
    else{
      for (int k = 0; k < effected_ci.count; k++){
        Ball* effected = grid_buffer[effected_ci.start + k];
        
        for (int j = 0; j < appl_ci.count; j++){
          Ball* applier = grid_buffer[appl_ci.start + j];
          if (applier_idx == cellnum && j == k) continue;   //It is the same ball, so no forces act on it
          apply_ball_forces(effected, applier);
        }
        apply_ball_resistances(effected);
      }
    }
  
  }
}
 
void apply_forces(Ball balls[]){
  assign_balls_to_grid(balls);
  
  for (int i = 0; i < GRID_CELLCOUNT; i++){
    apply_cell_forces(i);
  }
}

void apply_forces_parallel(Ball balls[]){
  //clock_t start = clock();
  assign_balls_to_grid(balls);
  //clock_t end = clock();
  //printf("Assignment took: %.2fms\n", (1000.0 * (end - start)) / CLOCKS_PER_SEC);

  #pragma omp parallel for schedule(dynamic)
  for (int i = 0; i < GRID_CELLCOUNT; i++) {
    apply_cell_forces(i);
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
            case TYPE_BLUE:     color = al_map_rgb(0, 102, 204); break;
            case TYPE_YELLOW:   color = al_map_rgb(255, 204, 0); break;
            case TYPE_GREEN:    color = al_map_rgb(0, 204, 102); break;
            case TYPE_PURPLE:   color = al_map_rgb(153, 50, 204); break;
            case TYPE_RED:      color = al_map_rgb(204, 0, 0); break;        
            case TYPE_CYAN:     color = al_map_rgb(0, 204, 204); break;
            case TYPE_ORANGE:   color = al_map_rgb(255, 128, 0); break;      
            case TYPE_PINK:     color = al_map_rgb(255, 105, 180); break;    
        }
        if (isnan(balls[i].x) || isnan(balls[i].y)) {
          printf("NaN detected in position: b[%d] type %d\n", i, balls[i].type);
        }
        al_draw_filled_circle(balls[i].x, balls[i].y, BALL_RADIUS, color);
    }
    al_flip_display();
}

void init_balls(Ball balls[], bool first) {
  if (first){
  randomize_color_forces();
  randomize_color_dist();
  }
  for (int t = 0; t < NUM_TYPES; t++) {
        for (int i = 0; i < BALLS_PER_TYPE; i++) {
            int idx = t * BALLS_PER_TYPE + i;
            balls[idx].x = rand() % WINDOW_WIDTH;
            balls[idx].y = rand() % WINDOW_HEIGHT;

            balls[idx].dx = rand_float(-MAX_SPEED, MAX_SPEED);
            balls[idx].dy = rand_float(-MAX_SPEED, MAX_SPEED);
            balls[idx].type = t;
        }
    }
    printf("FINISHED_INIT\n");
}

void exit_handl(){
  char str[] = "There was a segfault - may be due to incorrect Allegro install.\nExiting Gracefully...\n";
  int unused = write(2, str, sizeof(str));
  fprintf(stderr, "Segfault or other crash occurred.\n");
  (void) unused;
  exit(1);
}

int main() {
    signal(SIGSEGV, exit_handl);
    
    srand(10);
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

    ALLEGRO_TIMER *timer = al_create_timer(1.0 / FRAME_RATE);
    must_init(timer, "Timer");

    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    must_init(queue, "Queue");

    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_keyboard_event_source());

    Ball balls[TOTAL_BALLS];
    init_balls(balls, true);

    bool running = true;
    bool redraw = true;
    ALLEGRO_EVENT ev;
    al_start_timer(timer);

    while (running) {
        al_wait_for_event(queue, &ev);
        switch (ev.type){
          case ALLEGRO_EVENT_TIMER:
            clock_t start = clock();
            apply_forces_parallel(balls);
            clock_t end = clock();
            printf("Frame took: %.2fms\n", (1000.0 * (end - start)) / CLOCKS_PER_SEC);
            update_positions(balls);
            redraw = true;
            break;
          case ALLEGRO_EVENT_KEY_DOWN:
            if (ev.keyboard.keycode == ALLEGRO_KEY_R) {
              randomize_color_forces();
              randomize_color_dist();
              break;
            }
            if (ev.keyboard.keycode == ALLEGRO_KEY_I){
              init_balls(balls, false);
              break;
            }
          /* fallthrough */
          case ALLEGRO_EVENT_DISPLAY_CLOSE:
            printf("DISPLAY_CLOSE\n");
            running = false;
            break;
        }
        if (redraw && al_is_event_queue_empty(queue)) {
            draw_balls(balls);
            redraw = false;
        }
    }
    printf("EXITING\n");
    al_destroy_display(disp);
    al_destroy_event_queue(queue);
    al_destroy_timer(timer);
    exit(0);
}
