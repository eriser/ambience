/* voice.h */

#ifndef VOICE_H
#define VOICE_H

#include "osc.h"
#include "env.h"
#include "filter.h"

#define NUM_OSCILLATORS (3)

typedef struct voice_s
{
	oscillator_t	a_oscillators[NUM_OSCILLATORS];
	adsr_t			a_volume_envelopes[NUM_OSCILLATORS];
	filter_t		a_filters[NUM_OSCILLATORS];
	adsr_t			a_filter_envelopes[NUM_OSCILLATORS];
	real			a_filter_envelope_amounts[NUM_OSCILLATORS];
	real			filter_lfo_mod;

	int				midi_note;
	int				midi_velocity;
} voice_t;

void
voice_init
(voice_t    *voice,
 int         samplerate);

void
voice_reset
(voice_t    *voice);

int
voice_is_active
(voice_t    *voice);

int
voice_is_pressed
(voice_t    *voice);

void
voice_set_midi_note
(voice_t    *voice,
 int          midi_note);

void
voice_note_on
(voice_t    *voice);

void
voice_note_off
(voice_t    *voice);


real
voice_get_sample
(voice_t    *voice);

#endif
