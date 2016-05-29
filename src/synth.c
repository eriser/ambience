/* synth.c */

#include "synth.h"
#include "types.h"
#include "osc.h"
#include "env.h"
#include "voice.h"


/* parameter setter functions */
static
void
	set_parameter_osc_all_waveform
(synth_parameter_union_t	 value,
 voice_t					*voice)
{
	for (int i = 0; i < NUM_OSCILLATORS; i++)
	{
		voice->a_oscillators[i].waveform = value.waveform_value;
	}
}

static
void
	set_parameter_osc_1_waveform
(synth_parameter_union_t	 value,
 voice_t					*voice)
{
		voice->a_oscillators[0].waveform = value.waveform_value;
}

static
void
	set_parameter_osc_2_waveform
(synth_parameter_union_t	 value,
 voice_t					*voice)
{
		voice->a_oscillators[1].waveform = value.waveform_value;
}

static
void
	set_parameter_osc_3_waveform
(synth_parameter_union_t	 value,
 voice_t					*voice)
{
		voice->a_oscillators[2].waveform = value.waveform_value;
}

static
void
	set_parameter_osc_1_detune
(synth_parameter_union_t	 value,
 voice_t					*voice)
{
		voice->a_oscillators[0].detune = value.real_value;
}

static
void
	set_parameter_osc_2_detune
(synth_parameter_union_t	 value,
 voice_t					*voice)
{
		voice->a_oscillators[1].detune = value.real_value;
}

static
void
	set_parameter_osc_3_detune
(synth_parameter_union_t	 value,
 voice_t					*voice)
{
		voice->a_oscillators[2].detune = value.real_value;
}

static
void
	set_parameter_osc_all_cutoff
(synth_parameter_union_t 	 value,
 voice_t					*voice)
{
	for (int i = 0; i < NUM_OSCILLATORS; i++)
	{
        filter_set_cutoff
		    (&voice->a_filters[i],
             value.real_value);
	}
}

static
void
	set_parameter_osc_all_resonance
(synth_parameter_union_t 	 value,
 voice_t					*voice)
{
	for (int i = 0; i < NUM_OSCILLATORS; i++)
	{
        filter_set_resonance
		    (&voice->a_filters[i],
             value.real_value);
	}
}

static
void
	set_parameter_osc_all_gain
(synth_parameter_union_t 	 value,
 voice_t					*voice)
{
	for (int i = 0; i < NUM_OSCILLATORS; i++)
	{
		voice->a_oscillators[i].gain = value.real_value;
	}
}

static
void
	set_parameter_osc_all_volume_envelope_attack
(synth_parameter_union_t 	 value,
 voice_t					*voice)
{
	for (int i = 0; i < NUM_OSCILLATORS; i++)
	{
		voice->a_volume_envelopes[i].a_stage_value[ATTACK] = value.real_value;
	}
}

static
void
	set_parameter_osc_all_volume_envelope_decay
(synth_parameter_union_t	 value,
 voice_t					*voice)
{
	for (int i = 0; i < NUM_OSCILLATORS; i++)
	{
		voice->a_volume_envelopes[i].a_stage_value[DECAY] = value.real_value;
	}
}

static
void
	set_parameter_osc_all_volume_envelope_sustain
(synth_parameter_union_t 	 value,
 voice_t					*voice)
{
	for (int i = 0; i < NUM_OSCILLATORS; i++)
	{
		voice->a_volume_envelopes[i].a_stage_value[SUSTAIN] = value.real_value;
	}
}

static
void
	set_parameter_osc_all_volume_envelope_release
(synth_parameter_union_t	 value,
 voice_t					*voice)
{
	for (int i = 0; i < NUM_OSCILLATORS; i++)
	{
		voice->a_volume_envelopes[i].a_stage_value[RELEASE] = value.real_value;
	}
}

static
int
	get_free_voice_index
(synth_t	*synth)
{

	int free_voice = -1;

	for (int i = 0; i < NUM_VOICES; i++)
	{
		if (!voice_is_active(&synth->a_voices[i]))
		{
			free_voice = i;
			break;
		}
	}

	return free_voice;
}


void
	synth_init
(synth_t	*synth,
 int		 samplerate
 )
{
	synth->samplerate = samplerate;

	for (int i = 0; i < NUM_VOICES; i++)
	{
		voice_init
			(&synth->a_voices[i],
			 samplerate);
	}

	osc_init(&synth->lfo, samplerate);

	synth->lfo_to_volume = 0.0;
	synth->lfo_to_filter = 0.0;
}

void
	synth_note_on
(synth_t	*synth,
 int		 midi_note)
{
	int idx = get_free_voice_index(synth);

	if (idx < 0 || idx >= NUM_VOICES)
	{
		return;
	}

	voice_reset
		(&synth->a_voices[idx]);
	voice_set_midi_note
		(&synth->a_voices[idx], 
		 midi_note);
	voice_note_on
		(&synth->a_voices[idx]);
}

void
	synth_note_off
(synth_t	*synth,
 int		 midi_note)
{
	int idx = -1;

	for (int i = 0; i < NUM_VOICES; i++)
	{
		if (synth->a_voices[i].midi_note == midi_note &&
				voice_is_pressed(&synth->a_voices[i]))
		{
			idx = i;
			break;
		}
	}

	if (idx < 0 || idx >= NUM_VOICES)
	{
		return;
	}

	voice_note_off(&synth->a_voices[idx]);

}

void
synth_get_samples_buffered
(synth_t	*synth,
 real		*buffer,
 int		 buffer_size)
{
	for (int i = 0; i < buffer_size; i++)
	{
		buffer[i] = synth_get_sample(synth);
	}
}

real
	synth_get_sample
(synth_t	*synth)
{
	real sample = 0.0;

	real lfo_sample = osc_get_sample(&synth->lfo);

	for (int i = 0; i < NUM_VOICES; i++)
	{
		if (voice_is_active(&synth->a_voices[i]))
		{
			real filter_lfo_mod = lfo_sample * synth->lfo_to_filter;
			filter_lfo_mod = filter_lfo_mod >  1.0 ?  1.0 : filter_lfo_mod;
			filter_lfo_mod = filter_lfo_mod < -1.0 ? -1.0 : filter_lfo_mod;

			synth->a_voices[i].filter_lfo_mod = filter_lfo_mod;

			sample += voice_get_sample(&synth->a_voices[i]);
		}
	}

	/* divide by 4.0 to boost the signal a little */
	sample /= NUM_VOICES / 4.0;

	sample *= (lfo_sample * synth->lfo_to_volume + 1.0) * 0.5;

	sample = sample >  1.0 ?  1.0 : sample;
	sample = sample < -1.0 ? -1.0 : sample;

	return sample;
}

void
	synth_set_parameter
(synth_t					*synth,
 synth_parameter_type	 	 parameter,
 synth_parameter_union_t	 value
 )
{

	/* function pointer variable */
	void (*parameter_setter)(synth_parameter_union_t, voice_t*);

	switch (parameter)
	{
		case OSC_ALL_WAVEFORM:
			parameter_setter = & set_parameter_osc_all_waveform;
			break;
		case OSC_1_WAVEFORM:
			parameter_setter = & set_parameter_osc_1_waveform;
			break;
		case OSC_2_WAVEFORM:
			parameter_setter = & set_parameter_osc_2_waveform;
			break;
		case OSC_3_WAVEFORM:
			parameter_setter = & set_parameter_osc_3_waveform;
			break;
        case OSC_ALL_GAIN:
            parameter_setter = & set_parameter_osc_all_gain;
            break;
		case OSC_ALL_VOLUME_ENVELOPE_ATTACK:
			parameter_setter = & set_parameter_osc_all_volume_envelope_attack;
			break;
		case OSC_ALL_VOLUME_ENVELOPE_DECAY:
			parameter_setter = & set_parameter_osc_all_volume_envelope_decay;
			break;
		case OSC_ALL_VOLUME_ENVELOPE_SUSTAIN:
			parameter_setter = & set_parameter_osc_all_volume_envelope_sustain;
			break;
		case OSC_ALL_VOLUME_ENVELOPE_RELEASE:
			parameter_setter = & set_parameter_osc_all_volume_envelope_release;
			break;
        case OSC_1_DETUNE:
			parameter_setter = & set_parameter_osc_1_detune;
            break; 
        case OSC_2_DETUNE:
			parameter_setter = & set_parameter_osc_2_detune;
            break; 
        case OSC_3_DETUNE:
			parameter_setter = & set_parameter_osc_3_detune;
            break; 
		case OSC_ALL_CUTOFF:
			parameter_setter = & set_parameter_osc_all_cutoff;
			break;
		case OSC_ALL_RESONANCE:
			parameter_setter = & set_parameter_osc_all_resonance;
			break;

        case LFO_FREQUENCY:
            osc_set_frequency(&synth->lfo, value.real_value);
            return;
        case LFO_TO_VOLUME:
            synth->lfo_to_volume = value.real_value;
            return;
        case LFO_TO_FILTER:
            synth->lfo_to_filter = value.real_value;
            return;
		default:
			return;

	}


	for (int i = 0; i < NUM_VOICES; i++)
	{
		parameter_setter(value, &synth->a_voices[i]);
	}
}














