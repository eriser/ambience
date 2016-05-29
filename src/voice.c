/* voice.c */

#include <math.h>

#include "voice.h"
#include "osc.h"
#include "env.h"
#include "filter.h"

void
    voice_init
(voice_t    *voice,
int         samplerate)
{
    for (int i = 0; i < NUM_OSCILLATORS; i++)
    {
        osc_init
            (&(voice->a_oscillators[i]),
             samplerate);
        env_init
            (&(voice->a_volume_envelopes[i]),
             samplerate);
        filter_init
            (&(voice->a_filters[i]));
        env_init
            (&(voice->a_filter_envelopes[i]),
             samplerate);

        voice->a_filter_envelope_amounts[i] = 0.0;
    }
    voice->midi_note = -1;
    voice->midi_velocity = 0;
}

void
    voice_reset
(voice_t    *voice)
{
    for (int i = 0; i < NUM_OSCILLATORS; i++)
    {
        osc_reset
            (&(voice->a_oscillators[i]));
        env_reset
            (&(voice->a_volume_envelopes[i]));
        filter_reset
            (&(voice->a_filters[i]));
        env_reset
            (&(voice->a_filter_envelopes[i]));

        voice->a_filter_envelope_amounts[i] = 0.0;
    }
}

void
voice_note_on
(voice_t	*voice)
{
	for (int i = 0; i < NUM_OSCILLATORS; i++)
	{
		env_note_on(&voice->a_volume_envelopes[i]);
		env_note_on(&voice->a_filter_envelopes[i]);
	}
}

void
voice_note_off
(voice_t	*voice)
{
	for (int i = 0; i < NUM_OSCILLATORS; i++)
	{
		env_note_off(&voice->a_volume_envelopes[i]);
		env_note_off(&voice->a_filter_envelopes[i]);
	}
}

int
    voice_is_active
(voice_t    *voice)
{
    for (int i = 0; i < NUM_OSCILLATORS; i++)
    {
        if (voice->a_volume_envelopes[i].current_stage != OFF)
        {
            return 1;
        }

        if (voice->a_filter_envelopes[i].current_stage != OFF)
        {
            return 1;
        }
    }
    return 0;
}

int
    voice_is_pressed
(voice_t    *voice)
{
    for (int i = 0; i < NUM_OSCILLATORS; i++)
    {
        if (voice->a_volume_envelopes[i].current_stage != OFF && 
                voice->a_volume_envelopes[i].current_stage != RELEASE)
        {
            return 1;
        }

        if (voice->a_filter_envelopes[i].current_stage != OFF &&
                voice->a_filter_envelopes[i].current_stage != RELEASE)
        {
            return 1;
        }
    }
    return 0;
}

void
    voice_set_midi_note
(voice_t    *voice,
 int          midi_note)
{
    voice->midi_note = midi_note;
    real frequency = 440.0 * pow(2.0, (midi_note - 69.0) / 12.0);
    for (int i = 0; i < NUM_OSCILLATORS; i++)
    {
        osc_set_frequency
            (&(voice->a_oscillators[i]),
             frequency);
    }
}

real
    voice_get_sample
(voice_t    *voice)
{
    if (!voice_is_active(voice)) 
    {
        return 0.0;
    }

    real sum_sample = 0.0;

    for (int i = 0; i < NUM_OSCILLATORS; i++)
    {
        real sample = osc_get_sample
            (&(voice->a_oscillators[i]));

        real filter_mod = env_get_sample(&(voice->a_filter_envelopes[i])) * voice->a_filter_envelope_amounts[i] + voice->filter_lfo_mod;
        filter_mod = filter_mod >  1.0 ?  1.0 : filter_mod;
        filter_mod = filter_mod < -1.0 ? -1.0 : filter_mod;
        filter_set_cutoff_mod
            (&(voice->a_filters[i]),
             filter_mod);

        sample = filter_get_sample
            (&(voice->a_filters[i]),
             sample);

        sample *= env_get_sample
            (&(voice->a_volume_envelopes[i]));

        sum_sample += sample;
    }

    sum_sample /= (real) NUM_OSCILLATORS;

    return sum_sample;
}











