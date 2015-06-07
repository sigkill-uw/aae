#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#include "track.h"

#define BUFFER_SIZE		2048
#define SEMITONE_MULT	1.0594630943592953L
#define INITIAL_LENGTH	128

void get_line(FILE *input, char buffer[BUFFER_SIZE]);
int get_note_delta(char n);

compiled_track_t parse_track(FILE *input)
{
	char buffer[BUFFER_SIZE];
	char *pointer;

	long double tempo;
	long double base_tone;

	note_t note;
	long int delta;
	int d;

	compiled_track_t track;
	note_t *check;

	track.length = 0;
	track.notes = NULL;
	track.index = -1;
	track.remaining = 0.L;

	get_line(input, buffer);
	tempo = strtold(buffer, &pointer);
	if(pointer == buffer)
		return track;

	get_line(input, buffer);
	base_tone = strtold(buffer, &pointer);
	if(pointer == buffer)
		return track;

	tempo = 1.L / tempo * 60.L;

	while(!(feof(input) || ferror(input)))
	{
		get_line(input, buffer);

		note.duration = strtold(buffer, &pointer);
		if(pointer == buffer)
			continue;

		note.duration *= tempo;

		for(note.n_tones = 0; note.n_tones < MAX_TONES;)
		{
			while(isspace((int)*pointer))
				pointer ++;

			if(*pointer == '\0')
				break;

			delta = 12 * strtol(pointer, &pointer, 10);

			d = get_note_delta(*pointer);
			if(d == ~0)
			{
				while(pointer != '\0' && !isspace((int)*pointer))
					pointer ++;

				continue;
			}

			delta += d;
			pointer ++;

			if(*pointer != '\0' && !isspace((int)*pointer))
			{
				if(*pointer == '#')
				{
					delta ++;
				}
				else if(*pointer == '|')
				{
					delta --;
				}
				else
				{
					while(pointer != '\0' && !isspace((int)*pointer))
						pointer ++;

					continue;
				}

				pointer ++;
			}

			note.tones[note.n_tones ++] = base_tone * pow(SEMITONE_MULT, delta);
		}

		if(note.duration <= 0.L)
			continue;

		if(track.notes == NULL)
		{
			track.notes = (note_t *)malloc(sizeof(note_t) * INITIAL_LENGTH);
			if(track.notes == NULL)
				return track;
		}
		else if(track.length >= INITIAL_LENGTH && track.length == (track.length & -track.length))
		{
			check = (note_t *)realloc(track.notes, sizeof(note_t) * 2 * track.length);
			if(check == NULL)
				return track;

			track.notes = check;
		}

		track.notes[track.length ++] = note;
	}

	check = (note_t *)realloc(track.notes, sizeof(note_t) * track.length);
	if(check != NULL)
		track.notes = check;

	return track;
}

void free_track(compiled_track_t track)
{
	free(track.notes);
}

void get_line(FILE *input, char buffer[BUFFER_SIZE])
{
	int i, c;

	for(i = 0; i < BUFFER_SIZE - 1;)
	{
		c = fgetc(input);
		if(c == EOF || c == '\n')
			break;

		if(c != '\r')
			buffer[i ++] = c;
	}

	if(i == BUFFER_SIZE - 1 && !(c == EOF || c == '\r' || c == '\n'))
	{
		do
			c = getchar();
		while(!(c == EOF || c == '\n'));
	}

	buffer[i] = '\0';
}

int get_note_delta(char n)
{
	switch(n)
	{
		case 'a':
		case 'A':
			return 0;

		case 'b':
		case 'B':
			return 2;

		case 'c':
		case 'C':
			return -9;

		case 'd':
		case 'D':
			return -7;

		case 'e':
		case 'E':
			return -5;

		case 'f':
		case 'F':
			return -4;

		case 'g':
		case 'G':
			return -2;

		default:
			return ~0;
	}
}
