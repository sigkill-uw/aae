#ifndef _PARSE_H_
#define _PARSE_H_

#include "track.h"

compiled_track_t parse_track(FILE *input);
void free_track(compiled_track_t track);

#endif
