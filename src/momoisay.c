#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <termios.h>
#include <getopt.h>
#include "art/art.h"

#define VERSION "1.1.0-beta.1"
#define STATIC_V1_X 15
#define STATIC_V1_Y 110
#define ANIMATED_V1_X 35
#define ANIMATED_V1_Y 186
#define STATIC_VERSION 1
#define ANIMATED_VERSION 1
#define MAX_LENGTH 30

volatile int stop = 0;
struct termios oldt;

void help(){
    printf(
        "Make cute Momoi from Blue Archive say something!!!\n"
        "operations:\n"
        "    -h, --help                          Display this help message\n"
        "    -v, --version                       Show version information\n"
        "    -a, --animated <version> <text>     Cool animated version of cute Momoi (default version 1)\n"
        "    -s, --static <version> <text>       Display static version of cute Momoi (default version 1)\n"
        "    -l, --list                          List available versions for Momoi ASCII arts\n"
        "    -n, --no-text                      Display Momoi without a text bubble (Use in place of <text>)\n"
        "    <text>                              Text that cute Momoi will say!!! (default static version 1)\n"
    );
}

void version(){
    for(int i=0;i<STATIC_V1_X;i++){
        int y = strlen(momoi_static_v1[i]);
        for(int j=0;j<y;j++){
            printf("%c",momoi_static_v1[i][j]);
        }
        if(i==5||i==9){printf("------------------------");}
        if(i==6){printf("Momoisay v%s",VERSION);}
        if(i==7){printf("Author: Monasm");}
        if(i==8){printf("License: GPL-3.0 License");}
        printf("\n");
    }
}

int stoi(char *s){
    int num = 0,cnt = 0;
    while(*s!='\0'){
        num*=10;
        if(!('0'<=*s&&*s<='9')||(!cnt&&*s=='0')){
            return -1;
        }
        cnt++;
        num+=*s-'0';
        s++;
    }
    return num;
}

int get_line(char *argv[],int start,int end){
    int lines = 0,cnt = 0;
    if(end-start)lines++;
    for(int i=start;i<end;i++){
        char *str = argv[i];
        while(*str!='\0'){
            if(cnt>=MAX_LENGTH){
                cnt=0;lines++;
            }
            cnt++;str++;
        }
        cnt++;
    }
    return lines;
}

char** create_canvas(int x,int y){
    char **canvas = (char **)malloc(x*sizeof(char *));
    for(int i=0;i<x;i++){
        canvas[i] = (char *)malloc(y*sizeof(char));
    }
    return canvas;
}

void print_canvas(char **canvas,int x,int y){
    for(int i=0;i<x;i++){
        for(int j=0;j<y;j++){
            printf("%c",canvas[i][j]);
        }
        printf("\n");
    }
}

void free_canvas(char **canvas,int x){
    for(int i=0;i<x;i++){
        free(canvas[i]);
    }
    free(canvas);
}

int textlen(char *argv[],int start,int end){
    int length = 0;
    for(int i=start;i<end;i++)length+=strlen(argv[i])+1;
    if(length-1>MAX_LENGTH)return MAX_LENGTH;
    return length-1;
}

void restore_terminal(){
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    printf("\e[?25h");
    printf("\033[H\033[J");
    fflush(stdout);
}

void disable_input(){
    struct termios newt;
    tcgetattr(STDIN_FILENO,&oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO,TCSANOW,&newt);
}

void handle_exit(int sig){
    stop = 1;
}

void construct_animate_v1(const char **art[],char *argv[],int *intervals,int frames,int x,int y,int length,int lines,int start,int end,int nt){
    char **canvas=create_canvas(x,y+length+5);
    int pt1=(x+1)/2,pt2=((x+1)/2)+1,pts=3+y,ptt=pts,current_frame=0,cnt=0;
    signal(SIGINT,handle_exit);
    signal(SIGTSTP, handle_exit);
    signal(SIGHUP, handle_exit);
    signal(SIGTERM, handle_exit);
    disable_input();
    printf("\033[?25l");
    printf("\033[?1049h");
    fflush(stdout);
    while(!stop){
        printf("\033[2J\033[H");
        fflush(stdout);
        for(int i=0;i<x;i++){
            int len = strlen(art[current_frame][i]);
            for(int j=0;j<y+length+5;j++){
                if(j<len){
                    canvas[i][j]=art[current_frame][i][j];
                }
                else if(canvas[i][j]=='\0'){
                    canvas[i][j]=' ';
                }
                if(!i&&!cnt&&!nt){
                    if(j==y){
                        canvas[pt1--][j]='/';canvas[pt2++][j]='\\';
                    }
                    else if(j-1==y){
                        for(int cnt=0;cnt<lines/2;cnt++){
                            canvas[pt1--][j]='|';canvas[pt2++][j]='|';
                        }
                        pt2--;
                    }
                    else if(j+1==y+length+5){
                        for(int k=++pt1;k<=pt2;k++){
                            canvas[k][j]='|';
                        }
                        pt1++;
                    }
                    else{
                        canvas[pt1][j]='_';canvas[pt2][j]='_';
                    }
                }
            }
            if(!cnt&&!nt){
                for(int j=start;j<end;j++){
                    char *str = argv[j];
                    while(*str!='\0'){
                        if(cnt>=MAX_LENGTH){
                            pt1++;pts=ptt;cnt=0;
                        }
                        canvas[pt1][pts++] = *str;cnt++;str++;
                    }
                    if(cnt>MAX_LENGTH)continue;
                    canvas[pt1][pts++] = ' ';cnt++;
                }
            }
            cnt=1;
        }
        print_canvas(canvas,x,y+length+5);
        usleep(intervals[current_frame++]);
        current_frame=current_frame%frames;
    }
    printf("\033[?1049l");
    free_canvas(canvas,x);
    restore_terminal();
}

void construct_static_v1(const char *art[],char *argv[],int x,int y,int length,int lines,int start,int end,int nt){
    char **canvas=create_canvas(x,y+length+5);
    int pt1=(x+1)/2,pt2=((x+1)/2)+1,pts=3+y,ptt=pts;
    for(int i=0;i<x;i++){
        int len = strlen(art[i]);
        for(int j=0;j<y+length+5;j++){
            if(0<=i&&i<x&&0<=j&&j<y){
                if(j<len){
                    canvas[i][j]=art[i][j];
                }
            }
            else if(canvas[i][j]=='\0'){
                canvas[i][j]=' ';
            }
            if(!i&&!nt){
                if(j==y){
                    canvas[pt1--][j]='/';canvas[pt2++][j]='\\';
                }
                else if(j-1==y){
                    for(int cnt=0;cnt<lines/2;cnt++){
                        canvas[pt1--][j]='|';canvas[pt2++][j]='|';
                    }
                    pt2--;
                }
                else if(j+1==y+length+5){
                    for(int k=++pt1;k<=pt2;k++){
                        canvas[k][j]='|';
                    }
                    pt1++;
                }
                else{
                    canvas[pt1][j]='_';canvas[pt2][j]='_';
                }
            }
        }
    }
    int cnt=0;
    if(!nt){
        for(int i=start;i<end;i++){
            char *str = argv[i];
            while(*str!='\0'){
                if(cnt>=MAX_LENGTH){
                    pt1++;pts=ptt;cnt=0;
                }
                canvas[pt1][pts++] = *str;cnt++;str++;
            }
            if(cnt>MAX_LENGTH)continue;
            canvas[pt1][pts++] = ' ';cnt++;
        }
    }
    print_canvas(canvas,x,y+5+length);
    free_canvas(canvas,x);
}

int main(int argc,char *argv[]){
    int option;
    int mode = 0;
    int no_text = 0;
    int argctl = 0;
    int animated_version = 1;
    int static_version = 1;

    struct option long_options[] = {
        {"help", no_argument, NULL, 'h'},
        {"version", no_argument, NULL, 'v'},
        {"list", no_argument, NULL, 'l'},
        {"animated", optional_argument, NULL, 'a'},
        {"static", optional_argument, NULL, 's'},
        {"no-text", no_argument, NULL, 'n'},
        {NULL, 0, NULL, 0}
    };

    while((option = getopt_long(argc,argv,"hvlna::s::",long_options,NULL))!=-1){
        switch(option){
            case 'h':
                help();
                return 0;
            case 'v':
                version();
                return 0;
            case 'l':
                printf("static: ");
                for(int i=1;i<=STATIC_VERSION;i++){printf("%d",i);}
                printf("\n");
                printf("animated: ");
                for(int i=1;i<=ANIMATED_VERSION;i++){printf("%d",i);}
                printf("\n");
                return 0;
            case 'n':
                no_text = 1;
                break;
            case 'a':
                mode = 1;
                if(optarg && 0<stoi(optarg) && stoi(optarg)<=ANIMATED_VERSION){
                    argctl = 1;
                    animated_version = stoi(optarg);
                }
                break;
            case 's':
                mode = 0;
                if(optarg && 0<stoi(optarg) && stoi(optarg)<=STATIC_VERSION){
                    argctl = 1;
                    animated_version = stoi(optarg);
                }
                break;
            default:
                help();
                return 0;
        }
    }
    if(mode){
        if(animated_version==1){
            int length = 0,lines = 0;
            if(!no_text){
                length = textlen(argv,optind-argctl,argc);
                lines = get_line(argv,optind-argctl,argc);
            }
            if(lines<=30){
                if(lines&1)lines++;
                int frame[5] = {150000,75000,150000,75000,150000};
                construct_animate_v1(momoi_animated_v1,argv,frame,5,ANIMATED_V1_X,ANIMATED_V1_Y,length,lines,optind-argctl,argc,no_text);
                return 0;
            }
        }
    }
    else{
        if(static_version==1){
            int length = 0,lines = 0;
            if(!no_text){
                length = textlen(argv,optind-argctl,argc);
                lines = get_line(argv,optind-argctl,argc);
            }
            if(lines<=10){
                if(lines&1)lines++;
                construct_static_v1(momoi_static_v1,argv,STATIC_V1_X,STATIC_V1_Y,length,lines,optind-argctl,argc,no_text);
                return 0;
            }
        }
    }
}

  
