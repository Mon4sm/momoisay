#include "render.h"

#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <locale.h>
#include <unistd.h>
#include <ncurses.h>

static int use_color = 0;

static const struct{
    const char *name;
    short curses_color;
} COLOR_TABLE[COLOR_ART_COUNT] = {
    [COLOR_DEFAULT]     = {"default", -1},
    [COLOR_ART_RED]     = {"red",     COLOR_RED},
    [COLOR_ART_GREEN]   = {"green",   COLOR_GREEN},
    [COLOR_ART_YELLOW]  = {"yellow",  COLOR_YELLOW},
    [COLOR_ART_BLUE]    = {"blue",    COLOR_BLUE},
    [COLOR_ART_MAGENTA] = {"magenta", COLOR_MAGENTA},
    [COLOR_ART_CYAN]    = {"cyan",    COLOR_CYAN},
    [COLOR_ART_WHITE]   = {"white",   COLOR_WHITE},
};

const char *color_name(color c){
    if(c >= COLOR_ART_COUNT) return "";
    return COLOR_TABLE[c].name;
}

color to_color(const char *s){
    for(int i=0;i<COLOR_ART_COUNT;i++){
        if(!strcmp(s, COLOR_TABLE[i].name)) return i;
    }
    return COLOR_ART_COUNT;
}

int rand_between(int min, int max){
    return rand()%(max-min+1)+min;
}

int render_max_lines(int rows){
    int mid = (rows+1)/2;
    int top = 2*(mid-1);
    int bottom = 2*(rows-mid-2);
    return top < bottom ? top : bottom;
}

static void handle_signal(int sig){
    (void)sig;
    endwin();
    _exit(0);
}

void render_init(color c){
    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    nodelay(stdscr, TRUE);
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);
    if(c != COLOR_DEFAULT && has_colors()){
        start_color();
        use_default_colors();
        init_pair(1, COLOR_TABLE[c].curses_color, -1);
        use_color = 1;
    }
}

static char **canvas_create(int rows, int cols){
    char **canvas = malloc(rows*sizeof(char *));
    if(!canvas) return NULL;
    for(int i=0;i<rows;i++){
        canvas[i] = malloc(cols+1);
        if(!canvas[i]){
            while(i--) free(canvas[i]);
            free(canvas);
            return NULL;
        }
        memset(canvas[i], ' ', cols);
        canvas[i][cols] = '\0';
    }
    return canvas;
}

static void canvas_free(char **canvas, int rows){
    if(!canvas) return;
    for(int i=0;i<rows;i++) free(canvas[i]);
    free(canvas);
}

static char **template_create(const animation *anim, const speech *sp){
    int x = anim->rows, y = anim->cols;
    int length = sp->width, lines = sp->bubble_lines;
    char **tpl = canvas_create(x, y+length);
    if(!tpl) return NULL;
    if(!length) return tpl;
    int pt1 = (x+1)/2, pt2 = ((x+1)/2)+1;
    for(int j=0;j<y+length;j++){
        if(j==y){
            tpl[pt1--][j] = '/';
            tpl[pt2++][j] = '\\';
        }
        else if(j-1==y){
            for(int cnt=0;cnt<lines/2;cnt++){
                tpl[pt1--][j] = '|';
                tpl[pt2++][j] = '|';
            }
            pt2--;
        }
        else if(j+1==y+length){
            for(int k=++pt1;k<=pt2;k++){
                tpl[k][j] = '|';
            }
            pt1++;
        }
        else{
            tpl[pt1][j] = '_';
            tpl[pt2][j] = '_';
        }
    }
    for(size_t k=0;k<sp->count && pt1+(int)k < x;k++){
        cString *line = sp->lines[k];
        memcpy(&tpl[pt1+k][3+y], cstring_begin(line), cstring_size(line));
    }
    return tpl;
}

static void blit_frame(char **canvas, char **tpl, const char **art, int rows, int cols){
    for(int i=0;i<rows;i++){
        memcpy(canvas[i], tpl[i], cols+1);
        size_t len = strlen(art[i]);
        if(len > (size_t)cols) len = cols;
        memcpy(canvas[i], art[i], len);
    }
}

void render_run(const animation *anim, const speech *sp, int rounds){
    int cols = anim->cols + sp->width;
    char **tpl = template_create(anim, sp);
    char **canvas = canvas_create(anim->rows, cols);
    if(!tpl || !canvas){
        canvas_free(tpl, anim->rows);
        canvas_free(canvas, anim->rows);
        return;
    }
    const loop_policy *loop = &anim->loop;
    int frame = 0, replap = 0;
    if(loop->rep_frame >= 0) replap = rand_between(loop->rep_min, loop->rep_max);
    while(rounds != 0){
        int ch = getch();
        if(ch=='q' || ch=='Q'){
            endwin();
            exit(0);
        }
        erase();
        int px = (COLS - anim->view_cols - sp->width)/2;
        int py = (LINES - anim->rows)/2;
        blit_frame(canvas, tpl, anim->frames[frame], anim->rows, cols);
        for(int i=0;i<anim->rows;i++){
            if(use_color) attron(COLOR_PAIR(1));
            mvprintw(py+i, px, "%.*s", anim->cols, canvas[i]);
            if(use_color) attroff(COLOR_PAIR(1));
            printw("%s", canvas[i]+anim->cols);
        }
        refresh();
        usleep(anim->intervals[frame]);
        if(loop->rep_frame >= 0 && frame == loop->rep_frame){
            if(replap == 0){
                replap = rand_between(loop->rep_min, loop->rep_max);
                frame++;
            }
            else{
                frame = 0;
                replap--;
            }
        }
        else{
            frame++;
        }
        if(frame == anim->frame_count){
            frame = 0;
            if(rounds > 0) rounds--;
        }
    }
    canvas_free(canvas, anim->rows);
    canvas_free(tpl, anim->rows);
}
