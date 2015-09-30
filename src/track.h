#ifndef _TRACK_H_
#define _TRACK_H_

#define MAX_TONES		16

typedef struct
{
	int n_tones;
	long double tones[MAX_TONES];
	long double duration;
} note_t;

typedef struct
{
	int length;
	note_t *notes;

	int index;
	long double remaining;
} compiled_track_t;

#endif
