/* osc.c */

#include <math.h>
#include <stdlib.h>

#include "types.h"
#include "osc.h"


static
inline
void
update_increment
    (oscillator_t   *osc
    )
{
    osc->phase_increment = osc->frequency * 2.0 * M_PI / osc->samplerate;
}

static
inline
real
poly_blep
    (oscillator_t   *osc
    ,real            t
    )
{
    real dt = osc->phase_increment / (2.0 * M_PI);

    if (t < dt)
    {
        t /= dt;
        return (2.0 - t) * t - 1.0;
    }
    else if (t > 1.0 - dt)
    {
        t = (t - 1.0) / dt;
        return (2.0 + t) * t + 1.0;
    }
    else
    {
        return 0.0;
    }
}

void
osc_init
    (oscillator_t   *osc
    ,int             samplerate
    )
{
    osc->waveform       = SAW;
    osc->phase          = 0.0;
    osc->samplerate     = samplerate;
    osc->last_output    = 0.0;
    osc->gain           = 1.0;

    osc_set_frequency
        (osc
        ,440.0
        );

    osc_set_detune
        (osc
        ,0.0
        );

    update_increment(osc);
}

void
osc_set_frequency
    (oscillator_t   *osc
    ,real            frequency
    )
{
    osc->frequency = frequency;
    update_increment(osc);
}  

void
osc_set_detune
    (oscillator_t   *osc
    ,real            detune
    )
{
    osc->detune = detune;

    real new_frequency = osc->frequency * pow(2.0, (detune / 1200.0));
    osc_set_frequency
        (osc
        ,new_frequency
        );
}

void
osc_reset
    (oscillator_t   *osc
    )
{
    osc->phase          = 0.0;
    osc->last_output    = 0.0;
}

real
osc_get_sample
    (oscillator_t   *osc
    )
{
    real two_pi = 2.0 * M_PI;
    real t = osc->phase / two_pi;
    real value = 0.0;

    switch(osc->waveform)
    {
    case SINE:
        value = sin(osc->phase);
        break;
    case SAW:
        value = (2.0 * t) - 1.0;
        value -= poly_blep(osc, t);
        break;
    case SQUARE:
        if (osc->phase < M_PI)
        {
            value = 1.0;
        }
        else
        {
            value = -1.0;
        }
        value += poly_blep(osc, t);
        value -= poly_blep(osc, fmod(t + 0.5, 1.0));
        break;
    case TRIANGLE:
        if (osc->phase < M_PI)
        {
            value = 1.0;
        }
        else
        {
            value = -1.0;
        }
        value += poly_blep(osc, t);
        value -= poly_blep(osc, fmod(t + 0.5, 1.0));
        value = value * osc->phase_increment + (1.0 - osc->phase_increment) * osc->last_output;
        break;
    case WHITE_NOISE:
        value = 2.0 *((real)rand())/RAND_MAX - 1.0;
        break;
    }

    osc->phase += osc->phase_increment;
    while (osc->phase > two_pi)
    {
        osc->phase -= two_pi;
    }

    osc->last_output = value;

    value *= osc->gain;
            
    return value;
    
    
    
}
  


