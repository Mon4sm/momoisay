#ifndef SPEECH_H
#define SPEECH_H

#include <stddef.h>
#include "extern/cString.h"

#define SPEECH_WRAP_WIDTH 30

typedef struct speech{
    cString **lines;
    size_t count;
    int bubble_lines;
    int width;
} speech;

speech *speech_create(char *argv[], int start, int end);
void speech_free(speech *sp);

#endif
