// cluster_utils.h
#ifndef CLUSTER_UTILS_H
#define CLUSTER_UTILS_H

#include <stdbool.h>
#include <stdlib.h>

#define NUM_TYPES 6

#define MIN_COL_DIST 1
#define MAX_COL_DIST 100
#define MIN_FORCE -2
#define MAX_FORCE 2

typedef struct {
    float x, y;
    float dx, dy;
    int type;
} Ball;

extern int color_distances[NUM_TYPES][NUM_TYPES];
extern float color_forces[NUM_TYPES][NUM_TYPES];

int max_color_dist();
float rand_float(float min, float max);
int getRandomInt(int min, int max);
void randomize_color_dist(void);
void randomize_color_forces(void);

void must_init(bool test, const char *description);
float distance_squared(float dx, float dy);
float clamp_speed(float v, float max);

#endif
