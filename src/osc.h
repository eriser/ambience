/* osc.h */

#ifndef OSC_H
#define OSC_H

#include "types.h"

typedef enum 
{
    SINE = 0,
    SAW,
    SQUARE,
    TRIANGLE,
    WHITE_NOISE
} waveform_type;

typedef struct oscillator_s
{
    waveform_type   waveform;
    real            frequency;
    real            phase;
    int             samplerate;
    real            gain;
    real            detune;
    real            last_output;
    real            phase_increment;

} oscillator_t;

void
osc_init
    (oscillator_t   *osc
    ,int             samplerate
    );

void
osc_set_frequency
    (oscillator_t   *osc
    ,real            frequency
    );

void
osc_set_detune
    (oscillator_t   *osc
    ,real            detune
    );

real
osc_get_sample
    (oscillator_t   *osc
    );

void
osc_reset
    (oscillator_t   *osc
    );

#endif
