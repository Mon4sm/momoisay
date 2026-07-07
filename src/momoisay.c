#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <getopt.h>
#include "art/art.h"
#include "render.h"
#include "speech.h"

#define VERSION "1.1.1"

static const int STATIC_V1_INTERVALS[] = {75000};
static const int ANIMATED_V1_INTERVALS[] = {150000, 75000, 150000, 150000, 75000};
static const int ANIMATED_V2_INTERVALS[] = {70000, 70000, 70000, 1500000, 70000, 70000, 70000};
static const int ANIMATED_V3_INTERVALS[] = {70000, 70000, 70000, 70000, 70000, 70000, 70000, 70000};

static const animation STATIC_ARTS[] = {
    {momoi_static_v1, STATIC_V1_INTERVALS, 1, 15, 110, 38, LOOP_NONE},
};

static const animation ANIMATED_ARTS[] = {
    {momoi_animated_v1, ANIMATED_V1_INTERVALS, 5, 30, 187, 62, LOOP_NONE},
    {momoi_animated_v2, ANIMATED_V2_INTERVALS, 7, 30, 189, 62, {1, 5, 13}},
    {momoi_animated_v3, ANIMATED_V3_INTERVALS, 8, 30, 189, 62, LOOP_NONE},
};

#define STATIC_COUNT ((int)(sizeof(STATIC_ARTS)/sizeof(STATIC_ARTS[0])))
#define ANIMATED_COUNT ((int)(sizeof(ANIMATED_ARTS)/sizeof(ANIMATED_ARTS[0])))
#define FREESTYLE_MAX_LINES 10

enum { MODE_STATIC, MODE_ANIMATED, MODE_FREESTYLE };

static void help(void){
    printf(
        "Make cute Momoi from Blue Archive say something!!!\n"
        "operations:\n"
        "    -h, --help                          Display this help message\n"
        "    -v, --version                       Show version information\n"
        "    -a <version> <text>                 Cool animated version of cute Momoi (default version 1)\n"
        "    -f <text>                           Freestyle Momoi animation\n"
        "    -s <version> <text>                 Display static version of cute Momoi (default version 1)\n"
        "    -l, --list                          List available versions for Momoi ASCII arts\n"
        "    <text>                              Text that cute Momoi will say!!! (default static version 1)\n"
    );
}

static void version(void){
    printf("Momoisay v%s\n"
           "License: GPL-3.0 License\n", VERSION);
}

static void list(void){
    printf("static: ");
    for(int i=1;i<=STATIC_COUNT;i++) printf("%d ", i);
    printf("\n");
    printf("animated: ");
    for(int i=1;i<=ANIMATED_COUNT;i++) printf("%d ", i);
    printf("\n");
}

static int stoi(const char *s){
    int num = 0, cnt = 0;
    while(*s!='\0'){
        num *= 10;
        if(!('0'<=*s && *s<='9') || (!cnt && *s=='0')){
            return -1;
        }
        cnt++;
        num += *s-'0';
        s++;
    }
    return num;
}

static int randint(const int arr[], int size){
    return arr[rand()%size];
}

static void parse_version_arg(int argc, char *argv[], int max, int *version, int *ctl, int *argctl){
    if(!*ctl) *argctl = 0;
    if(argc <= 2) return;
    int value = stoi(argv[2]);
    if(0 < value && value <= max && !*ctl){
        *ctl = 1;
        *argctl = 1;
        *version = value;
    }
}

static void freestyle(const speech *sp){
    int select = rand_between(0, ANIMATED_COUNT-1);
    while(1){
        if(select==0){
            render_run(&ANIMATED_ARTS[0], sp, rand_between(3, 5));
            select = 2;
        }
        else if(select==1){
            render_run(&ANIMATED_ARTS[1], sp, rand_between(1, 3));
            select = randint((int []){0, 2}, 2);
        }
        else if(select==2){
            render_run(&ANIMATED_ARTS[2], sp, rand_between(3, 5));
            select = randint((int []){1}, 1);
        }
    }
}

int main(int argc, char *argv[]){
    srand(time(NULL));

    int option;
    int mode = MODE_STATIC;
    int ctl = 0;
    int argctl = 0;
    int animated_version = 1;
    int static_version = 1;

    struct option long_options[] = {
        {"help", no_argument, NULL, 'h'},
        {"version", no_argument, NULL, 'v'},
        {"list", no_argument, NULL, 'l'},
        {NULL, 0, NULL, 0}
    };

    while((option = getopt_long(argc, argv, "hvla::s::f::", long_options, NULL))!=-1){
        switch(option){
            case 'h':
                help();
                return 0;
            case 'v':
                version();
                return 0;
            case 'l':
                list();
                return 0;
            case 'a':
                mode = MODE_ANIMATED;
                parse_version_arg(argc, argv, ANIMATED_COUNT, &animated_version, &ctl, &argctl);
                break;
            case 's':
                mode = MODE_STATIC;
                parse_version_arg(argc, argv, STATIC_COUNT, &static_version, &ctl, &argctl);
                break;
            case 'f':
                mode = MODE_FREESTYLE;
                if(!ctl) argctl = 0;
                break;
            default:
                help();
                return 0;
        }
    }

    speech *sp = speech_create(argv, optind+argctl, argc);
    if(!sp){
        fprintf(stderr, "momoisay: out of memory\n");
        return 1;
    }

    const animation *anim;
    int max_lines;
    if(mode==MODE_FREESTYLE){
        anim = NULL;
        max_lines = FREESTYLE_MAX_LINES;
    }
    else if(mode==MODE_ANIMATED){
        anim = &ANIMATED_ARTS[animated_version-1];
        max_lines = render_max_lines(anim->rows);
    }
    else{
        anim = &STATIC_ARTS[static_version-1];
        max_lines = render_max_lines(anim->rows);
    }

    if(sp->bubble_lines > max_lines){
        fprintf(stderr, "momoisay: text is too long for this art\n");
        speech_free(sp);
        return 1;
    }

    render_init();
    if(mode==MODE_FREESTYLE){
        freestyle(sp);
    }
    else{
        render_run(anim, sp, -1);
    }
    speech_free(sp);
    return 0;
}
