#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <signal.h>
#include <pthread.h>
#include "cluster_utils.h"

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 700

#define GRID_SIZE 50 //MUST BE BIGGER THAN MAX_COL_DIST
#define GRID_COLS (WINDOW_WIDTH / GRID_SIZE)
#define GRID_ROWS (WINDOW_HEIGHT / GRID_SIZE)
#define GRID_CELLCOUNT (GRID_COLS * GRID_ROWS)

#define BALL_RADIUS 1
#define NUM_TYPES 4
#define BALLS_PER_TYPE 200
#define TOTAL_BALLS (NUM_TYPES * BALLS_PER_TYPE)

#define REPULSION 5.0f
#define MIN_DIST 1.0f

//Higher number is less, fricton 1 is equivalent zero friction
#define FRICTION 0.99f
#define MAX_SPEED 10.0f

#define RAND_FORCES true
#define RAND_DIST false

#define NUM_WORKERS 50

enum {
    TYPE_BLUE = 0,
    TYPE_YELLOW,
    TYPE_GREEN,
    TYPE_PURPLE
};


Ball* grid_buffer[TOTAL_BALLS];

//Each grid cell contains grid_buffer start index, and count of balls in the cell
typedef struct{
  int start;
  int count;
} CellIndex;

CellIndex cell_Indicies[GRID_CELLCOUNT];

void clear_grid(void){
  for (int i = 0; i < GRID_CELLCOUNT; i++){
    cell_Indicies[i].count = 0;
    cell_Indicies[i].start = 0;
  }
}

  //for (int i = 0; i < GRID_ROWS; i++){
  //  for (int j = 0; j < GRID_COLS; j++){
  //    BallNode* node = grid_arr[i][j];
  //    while (node){
  //      BallNode* next = node->next;
  //      free(node);
  //      node = next;
  //    }
  //    grid_arr[i][j] = NULL;
  //  }
  //}
  //}

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
    printf("WARNING: Ball out of grid bounds: (%.1f, %.1f)\n", b->x, b->y);
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

  //for (int i = 0; i < GRID_CELLCOUNT; i++){
  //  CellIndex* ci = cell_Indicies[i];
  //  ci->start = prev->start + prev->count;
  //  for (int j = 0; j < TOTAL_BALLS; j++){
  //    Ball* b = &balls[j];
  //    int grid_row = ((int)b->y / GRID_SIZE);
  //    int grid_col = ((int)b->x / GRID_SIZE);
  //    if (grid_row < 0 || grid_row >= GRID_ROWS || grid_col < 0 || grid_col >= GRID_COLS){
  //      printf("ERR IN ASIGNING\n");
  //    }
  //    //The ball is not within the cell
  //    if (grid_row % GRID_ROW && grid_col % GRID_COL) continue;
  //    grid_buffer[ci->start + ci->count] = b;
  //    ci->count++;
  //  }
  //  prev = ci;
  //}
}

// Returns the force magnitude between two types at a distance
float compute_force(int type1, int type2, float dist) {
    // Distance-based custom interaction logic
    //float col_forces = color_forces[type1][type2];
    //int col_dist = color_distances[type1][type2];
    //return (dist < col_dist) ? -REPULSION / dist : col_forces;
    const float max_influence = color_distances[type1][type2];
    if (dist > max_influence){return 0;}
    if (dist < 0.01f) dist = 0.01f; // Avoid division by zero

    const float ideal = max_influence * 2; //the midpoint between the max_ifluence and 0 distance is the maximum force.
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
  if ((dist_sq < 0.1)) return; // avoid div by 0

  float max_dist = color_distances[effected->type][applier->type];
  if (dist_sq > max_dist * max_dist){return;}
  float dist = sqrtf(dist_sq);
  float force = compute_force(effected->type, applier->type, dist);
  dx /= dist;
  dy /= dist;

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
    
    for (int k = 0; k < effected_ci.count; k++){
      Ball* effected = grid_buffer[effected_ci.start + k];
      
      for (int j = 0; j < appl_ci.count; j++){
        Ball* applier = grid_buffer[appl_ci.start + j];
        if (i == 5 && j == k) continue;   //It is the same ball, so no forces act on it
        apply_ball_forces(effected, applier);
      }
      apply_ball_resistances(effected);
    }
  }
}
 
void apply_forces(Ball balls[]){
  assign_balls_to_grid(balls);
  
  for (int i = 0; i < GRID_CELLCOUNT; i++){
    apply_cell_forces(i);
  }
}

void* forces_worker(void* args){
  int id = *(int *)args;
  for (int i = id; i < GRID_CELLCOUNT; i+= NUM_WORKERS){
    apply_cell_forces(i);
  }
  return NULL;
}

void apply_forces_parallel(Ball balls[]){
  assign_balls_to_grid(balls);
  pthread_t threads[NUM_WORKERS];
  int ids[NUM_WORKERS];

  for (int i = 0; i < NUM_WORKERS; i++){
    ids[i] = i;
    pthread_create(&threads[i], NULL, forces_worker, &ids[i]);
  }
  
  for (int i = 0; i < NUM_WORKERS; i++){
    pthread_join(threads[i], NULL);
  }
}
    
    //for (int i = 0; i < TOTAL_BALLS; i++) {
    //    Ball *a = &balls[i];
    //    int a_type = a->type;
    //    for (int j = 0; j < TOTAL_BALLS; j++) {
    //        if (i == j) continue;
    //        Ball *b = &balls[j];

    //        float dx = b->x - a->x;
    //        float dy = b->y - a->y;
    //        
    //        //Wraparound logic for forces
    //        if (dx > WINDOW_WIDTH / 2) dx -= WINDOW_WIDTH;
    //        else if (dx < -WINDOW_WIDTH / 2) dx += WINDOW_WIDTH;
    //        if (dy > WINDOW_HEIGHT / 2) dy -= WINDOW_HEIGHT;
    //        else if (dy < -WINDOW_HEIGHT / 2) dy += WINDOW_HEIGHT;
    //        
    //        float dist_sq = distance_squared(dx, dy);
    //        if ((dist_sq < 0.1)) continue; // avoid div by 0
    //        float max_dist = color_distances[a_type][b->type];
    //        if (dist_sq > max_dist * max_dist){continue;}
    //        
    //        float dist = sqrtf(dist_sq);
    //        float force = compute_force(a_type, b->type, dist);
    //        dx /= dist;
    //        dy /= dist;

    //        a->dx += dx * force;
    //        a->dy += dy * force;
    //    }

    //    // Apply friction and clamp velocity
    //    a->dx *= FRICTION;
    //    a->dy *= FRICTION;
    //    const float speed_sq = a->dx * a->dx + a->dy * a->dy;
    //    if (speed_sq > MAX_SPEED * MAX_SPEED) {
    //      float speed = sqrtf(speed_sq);
    //      a->dx = (a->dx / speed) * MAX_SPEED;
    //      a->dy = (a->dy / speed) * MAX_SPEED;
    //    }
    //}


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
            case TYPE_PURPLE: color = al_map_rgb(128, 0, 128); break;
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

void exit_handl(){
  char str[] = "There was a segfault - may be due to incorrect Allegro install.\nExiting Gracefully...\n";
  int unused = write(2, str, sizeof(str));
  fprintf(stderr, "Segfault or other crash occurred.\n");
  (void) unused;
  exit(1);
}

int main() {
    signal(SIGSEGV, exit_handl);
    
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
            if (ev.keyboard.keycode == ALLEGRO_KEY_I){
              init_balls(balls);
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
