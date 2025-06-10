//cluster_utils.c
#include "cluster_utils.h"
#include <stdio.h>
#include <unistd.h>

int color_distances[NUM_TYPES][NUM_TYPES] = {
//    {60, 60, 60, 60, 60, 60, 60, 60}, 
//    {60, 60, 60, 60, 60, 60, 60, 60}, 
//    {60, 60, 60, 60, 60, 60, 60, 60}, 
//    {60, 60, 60, 60, 60, 60, 60, 60}, 
//    {60, 60, 60, 60, 60, 60, 60, 60}, 
//    {60, 60, 60, 60, 60, 60, 60, 60}, 
//    {60, 60, 60, 60, 60, 60, 60, 60}, 
//    {60, 60, 60, 60, 60, 60, 60, 60}, 
};
float color_forces[NUM_TYPES][NUM_TYPES] = {
//  {1, -1, 0, 1, 1, -1, 0, 1}, 
//  {1, -1, 0, 1, 1, -1, 0, 1}, 
//  {1, -1, 0, 1, 1, -1, 0, 1}, 
//  {1, -1, 0, 1, 1, -1, 0, 1}, 
//  {1, -1, 0, 1, 1, -1, 0, 1}, 
//  {1, -1, 0, 1, 1, -1, 0, 1}, 
//  {1, -1, 0, 1, 1, -1, 0, 1}, 
//  {1, -1, 0, 1, 1, -1, 0, 1}, 
};


int max_color_dist(){
  int max_dist = 0;
  for (int i = 0; i < NUM_TYPES; i++){
    for (int j = 0; j < NUM_TYPES; j++){
      if (color_distances[i][j] > max_dist){
        max_dist = color_distances[i][j];
      }
    }
  }
  return max_dist;
}

float rand_float(float min, float max) {
    return min + ((float)rand() / (float)RAND_MAX) * (max - min);
}

int getRandomInt(int min, int max) {
    return rand() % (max - min + 1) + min;
}

void randomize_color_dist(void){
  for (int i = 0; i < NUM_TYPES; i++){
    for (int j = 0; j < NUM_TYPES; j++){
      color_distances[i][j] = getRandomInt(MIN_COL_DIST, MAX_COL_DIST);
    }
  }
}

void randomize_color_forces(void){
  for (int i = 0; i < NUM_TYPES; i++){
    for (int j = 0; j < NUM_TYPES; j++){
      color_forces[i][j] = rand_float(MIN_FORCE, MAX_FORCE);
    }
  }
}

void must_init(bool test, const char *description) {
    if (!test) {
        printf("Failed to initialize: %s\n", description);
        _exit(1);
    }
}

float distance_squared(float dx, float dy) {
    return dx * dx + dy * dy;
}

float clamp_speed(float v, float max) {
    return (v > max) ? max : ((v < -max) ? -max : v);
}

