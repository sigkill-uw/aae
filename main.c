#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <SDL.h>

#include "track.h"
#include "parse.h"

#define CUT_IN	5e-2

#undef main

#ifndef M_PI
	#define M_PI	3.141592653589793L
#endif

#define AMP	32000

int16_t i16_sin(long double x)
{
	return (int16_t)(sin(x) / 2 * AMP);
}

int rate;
int pos;
long double fre;
void callback(void *user, uint8_t *stream, int len)
{
	int i, j, k;
	int16_t *buffer;
	compiled_track_t *track = user;
	long double mul;

	(void)user;

	buffer = (int16_t *)stream;
	len /= 2;

	for(i = 0; i < len; i ++)
	{
		if(track->remaining <= 0)
		{
			track->index ++;
			if(track->index >= track->length)
			{
				track->index = 0;
			}

			track->remaining = track->notes[track->index].duration;
		}

		track->remaining -= 1.L / rate;

		if(track->remaining <= CUT_IN / 4)
		{
			mul = 0;
		}
		if(track->remaining <= CUT_IN)
		{
			mul = track->remaining / CUT_IN;
		}
		else if(track->notes[track->index].duration - track->remaining <= CUT_IN)
		{
			mul = (track->notes[track->index].duration - track->remaining) / CUT_IN;
		}
		else
		{
			mul = 1;
		}

		if(track->notes[track->index].n_tones == 0)
		{
			buffer[i] = 0;
			continue;
		}

		k = 0;
		for(j = 0; j < track->notes[track->index].n_tones; j ++)
		{
			k += i16_sin(2 * M_PI * pos * track->notes[track->index].tones[j] / rate);
		}

		k /= track->notes[track->index].n_tones;

		buffer[i] = (int16_t)(k * mul);

		pos ++;
	}
}

int main(int argc, char **argv)
{
	int i, j;
	FILE *input;
	compiled_track_t track;

	SDL_AudioSpec want, have;
	SDL_AudioDeviceID dev;

	if(argc < 2)
	{
		puts("need file");
		return 0;
	}

	input = fopen(argv[1], "r");
	if(!input)
		return 1;

	track = parse_track(input);
	fclose(input);

	printf("Track has length %d\n\n", track.length);
	for(i = 0; i < track.length; i ++)
	{
		printf("Note %d has duration %Lf, %d tones\n", i + 1, track.notes[i].duration, track.notes[i].n_tones);
		for(j = 0; j < track.notes[i].n_tones; j ++)
			printf("%Lf\t", track.notes[i].tones[j]);

		puts("\n");
	}

	if(SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		puts("failure");
	}

	printf("%d\n", SDL_GetNumAudioDevices(0));

	want.freq = 48000;
	want.format = AUDIO_S16SYS;
	want.channels = 1;
	want.samples = 1024;
	want.callback = callback;
	want.userdata = &track;

	dev = SDL_OpenAudioDevice(NULL, 0, &want, &have, SDL_AUDIO_ALLOW_ANY_CHANGE);

	rate = have.freq;
	fre = 440.0 / rate;

	SDL_PauseAudioDevice(dev, 0);

	getchar();

	SDL_PauseAudioDevice(dev, 1);

	free_track(track);

	SDL_Quit();

	return 0;
}
