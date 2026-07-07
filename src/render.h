#ifndef RENDER_H
#define RENDER_H

#include "speech.h"

#define LOOP_NONE {-1, 0, 0}

typedef enum color{
    COLOR_DEFAULT = 0,
    COLOR_ART_RED,
    COLOR_ART_GREEN,
    COLOR_ART_YELLOW,
    COLOR_ART_BLUE,
    COLOR_ART_MAGENTA,
    COLOR_ART_CYAN,
    COLOR_ART_WHITE,
    COLOR_ART_COUNT
} color;

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

color to_color(const char *s);
const char *color_name(color c);
void render_init(color c);
int render_max_lines(int rows);
void render_run(const animation *anim, const speech *sp, int rounds);
int rand_between(int min, int max);

#endif
