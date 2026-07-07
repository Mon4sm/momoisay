#ifndef RENDER_H
#define RENDER_H

#include "speech.h"

#define LOOP_NONE {-1, 0, 0}

typedef struct loop_policy{
    int rep_frame;
    int rep_min;
    int rep_max;
} loop_policy;

typedef struct animation{
    const char ***frames;
    const int *intervals;
    int frame_count;
    int rows;
    int cols;
    int view_cols;
    loop_policy loop;
} animation;

void render_init(void);
int render_max_lines(int rows);
void render_run(const animation *anim, const speech *sp, int rounds);
int rand_between(int min, int max);

#endif
