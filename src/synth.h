/* synth.h */

#ifndef SYNTH_H
#define SYNTH_H

#include "synth.h"
#include "types.h"
#include "osc.h"
#include "env.h"
#include "voice.h"

#define NUM_VOICES (32)
#define BUFFER_SIZE (8192)

typedef enum
{
	OSC_ALL_WAVEFORM,                   
	OSC_1_WAVEFORM,                     
	OSC_2_WAVEFORM,                     
	OSC_3_WAVEFORM,                     
	OSC_ALL_GAIN,                       
	OSC_1_GAIN,                         
	OSC_2_GAIN,                         
	OSC_3_GAIN,                         
	OSC_ALL_DETUNE,                            
	OSC_1_DETUNE,                              
	OSC_2_DETUNE,                              
	OSC_3_DETUNE,                              
	OSC_ALL_VOLUME_ENVELOPE_ATTACK,           
	OSC_1_VOLUME_ENVELOPE_ATTACK,             
	OSC_2_VOLUME_ENVELOPE_ATTACK,             
	OSC_3_VOLUME_ENVELOPE_ATTACK,             
	OSC_ALL_VOLUME_ENVELOPE_DECAY,            
	OSC_1_VOLUME_ENVELOPE_DECAY,              
	OSC_2_VOLUME_ENVELOPE_DECAY,              
	OSC_3_VOLUME_ENVELOPE_DECAY,             
	OSC_ALL_VOLUME_ENVELOPE_SUSTAIN,         
	OSC_ALL_VOLUME_ENVELOPE_RELEASE,          
	OSC_1_VOLUME_ENVELOPE_RELEASE,            
	OSC_2_VOLUME_ENVELOPE_RELEASE,            
	OSC_3_VOLUME_ENVELOPE_RELEASE,            
	OSC_ALL_FILTER_ENVELOPE_ATTACK,           
	OSC_ALL_FILTER_ENVELOPE_DECAY,            
	OSC_ALL_FILTER_ENVELOPE_SUSTAIN,          
	OSC_ALL_FILTER_ENVELOPE_RELEASE,          
	OSC_ALL_FILTER_ENVELOPE_AMOUNT,     
	OSC_ALL_CUTOFF,                     
	OSC_ALL_RESONANCE,                  
	LFO_FREQUENCY,    
	LFO_TO_VOLUME,    
	LFO_TO_FILTER,    
	NUM_PARAMETERS
} synth_parameter_type;


typedef struct synth_s
{
	int             samplerate;
	voice_t         a_voices[NUM_VOICES];
	oscillator_t    lfo;
	real            lfo_to_volume;
	real            lfo_to_filter;

	real			buffer[BUFFER_SIZE];
} synth_t;


typedef union synth_parameter_union
{
	waveform_type   waveform_value;
	real            real_value;
} synth_parameter_union_t;

void
synth_init
(synth_t    *synth,
 int         samplerate
 );

void
synth_note_on
(synth_t    *synth,
 int         midi_note);

void
synth_note_off
(synth_t    *synth,
 int         midi_note);

void
synth_get_samples_buffered
(synth_t    *synth,
 real       *buffer,
 int         buffer_size);


real
synth_get_sample
(synth_t    *synth);

void
synth_set_parameter
(synth_t                *synth,
 synth_parameter_type    parameter,
 synth_parameter_union_t value
 );







#endif
