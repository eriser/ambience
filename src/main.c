
#include <stdio.h>

#include "synth.h"
#include "osc.h"

int main()
{

    int samplerate = 44100;

#if 1
    synth_t synth;
    synth_init(&synth, samplerate);
   
    synth_note_on(&synth, 60);
    for (int i = 0; i < 20000; i++)
    {
        synth_get_sample(&synth);
    }
 
    for (int i = 0; i < 200; i++)
    {
        printf("%.3f ", synth_get_sample(&synth));
    }

    synth_note_off(&synth, 60);

#endif

#if 0
	oscillator_t osc;
	osc_init(&osc, samplerate);

    for (int i = 0; i < 20; i++)
    {
        printf("%.3f ", osc_get_sample(&osc));
    }

#endif

#if 0
	adsr_t	env;
	env_init(&env, samplerate);

	env_note_on(&env);
    for (int i = 0; i < 3000; i++)
    {
        env_get_sample(&env);
    }
	
    for (int i = 0; i < 20; i++)
    {
        printf("%.3f ", env_get_sample(&env));
    }
#endif

    return 0;
}

