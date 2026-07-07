#include "speech.h"

#include <stdlib.h>
#include <string.h>

static int text_width(char *argv[], int start, int end){
    int length = 0;
    for(int i=start;i<end;i++) length += strlen(argv[i])+1;
    if(length-1 > SPEECH_WRAP_WIDTH) return SPEECH_WRAP_WIDTH;
    return length-1;
}

static int count_lines(char *argv[], int start, int end){
    int lines = 0, cnt = 0;
    if(end-start) lines++;
    for(int i=start;i<end;i++){
        for(char *s=argv[i]; *s!='\0'; s++){
            if(cnt>=SPEECH_WRAP_WIDTH){
                cnt = 0;
                lines++;
            }
            cnt++;
        }
        cnt++;
    }
    return lines;
}

static int push_line(speech *sp, cString *line){
    cString **grown = realloc(sp->lines, (sp->count+1)*sizeof(cString *));
    if(!grown) return 0;
    sp->lines = grown;
    sp->lines[sp->count++] = line;
    return 1;
}

static int wrap_lines(speech *sp, char *argv[], int start, int end){
    if(end-start <= 0) return 1;
    cString *cur = cstring_init("");
    if(!cur) return 0;
    int cnt = 0;
    char ch[2] = {0, 0};
    for(int i=start;i<end;i++){
        for(char *s=argv[i]; *s!='\0'; s++){
            if(cnt>=SPEECH_WRAP_WIDTH){
                if(!push_line(sp, cur)){
                    cstring_free(cur);
                    return 0;
                }
                cur = cstring_init("");
                if(!cur) return 0;
                cnt = 0;
            }
            ch[0] = *s;
            cstring_push_back(cur, ch);
            cnt++;
        }
        if(cnt>SPEECH_WRAP_WIDTH) continue;
        cstring_push_back(cur, " ");
        cnt++;
    }
    if(!push_line(sp, cur)){
        cstring_free(cur);
        return 0;
    }
    return 1;
}

speech *speech_create(char *argv[], int start, int end){
    speech *sp = calloc(1, sizeof(speech));
    if(!sp) return NULL;
    sp->width = 5+text_width(argv, start, end);
    if(sp->width <= 5) sp->width = 0;
    sp->bubble_lines = count_lines(argv, start, end);
    if(sp->bubble_lines & 1) sp->bubble_lines++;
    if(!wrap_lines(sp, argv, start, end)){
        speech_free(sp);
        return NULL;
    }
    return sp;
}

void speech_free(speech *sp){
    if(!sp) return;
    for(size_t i=0;i<sp->count;i++) cstring_free(sp->lines[i]);
    free(sp->lines);
    free(sp);
}
