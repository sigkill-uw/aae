# aae
A very simple synthesizer application using SDL2. Produces simple melodies using sine waves.

## Building
`cd src; make` to create the `aae` executable.

## Playing Music
Example tracks reside in `tracks/`.
Each track has a 2-line preamble containing a tempo (in BPM) and a base tone for middle A (in Hz).
These two lines can be either integers or floating-point values.
Thereafter, each "note" is expressed as an integer specifying duration (in beats), followed by up to 16 tone values of the form "nCf",
where n is an octave number (0 being the "middle" octave), C is a note value from A-G, and f is either "#", "|", or "" indicating sharp, flat, or natural.
Rests can be specified by a duration followed by no tones.

Running `aae` with the name of a track file will play the file over the default output device, using sinusodial waves.
