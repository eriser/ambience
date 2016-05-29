/* env.c */

#include <math.h>

#include "env.h"
#include "types.h"


static
void
	calculate_multiplier
(adsr_t	*adsr,
 real startlevel,
 real endlevel,
 unsigned long long length_in_samples)
{
	adsr->multiplier = 1.0 + (log(endlevel) - log(startlevel)) / (real)length_in_samples;
}

static
void
	enter_stage
(adsr_t		*adsr,
 stage_type	 new_stage
 )
{
	if (adsr->current_stage == new_stage)
	{
		return;
	}

	adsr->current_stage = new_stage;
	adsr->current_sample_index = 0;

	if (adsr->current_stage == OFF ||
			adsr->current_stage == SUSTAIN)
	{
		adsr->next_stage_sample_index = 0;		
	}
	else
	{
		adsr->next_stage_sample_index = (unsigned long long) (adsr->a_stage_value[adsr->current_stage] * (real)adsr->samplerate);
	}

	if (new_stage == OFF)
	{
		adsr->current_level = 0.0;
		adsr->multiplier = 1.0;
	}

	else if (new_stage == ATTACK)
	{
		adsr->current_level = adsr->min_level;
		calculate_multiplier
			(adsr,
			 adsr->current_level,
			 1.0,
			 adsr->next_stage_sample_index);
	}

	else if (new_stage == DECAY)
	{
		adsr->current_level = 1.0;
		calculate_multiplier
			(adsr,
			 adsr->current_level,
			 fmax(adsr->a_stage_value[SUSTAIN], adsr->min_level),
			 adsr->next_stage_sample_index);
	}

	else if (new_stage == SUSTAIN)
	{
		adsr->current_level = adsr->a_stage_value[SUSTAIN];
		adsr->multiplier = 1.0;
	}
	else if (new_stage == RELEASE)
	{
		calculate_multiplier
			(adsr,
			 adsr->current_level, 
			 adsr->min_level, 
			 adsr->next_stage_sample_index);
	}


}

void
	env_init
(adsr_t *adsr,
 int     samplerate
 )   
{

	adsr->a_stage_value[OFF]        = 0.0;
	adsr->a_stage_value[ATTACK]     = 0.1;
	adsr->a_stage_value[DECAY]      = 0.5;
	adsr->a_stage_value[SUSTAIN]    = 0.7;
	adsr->a_stage_value[RELEASE]    = 2.0;

	adsr->samplerate    = samplerate;

	adsr->current_level = ENV_MIN_LEVEL;
	adsr->min_level		= ENV_MIN_LEVEL;
	adsr->current_stage = OFF;
	adsr->multiplier    = 1.0;
	adsr->current_sample_index      = 0;
	adsr->next_stage_sample_index   = 0;
}

void
	env_reset
(adsr_t	*adsr)
{
	adsr->current_stage = OFF;
	adsr->current_level	= adsr->min_level;
	adsr->multiplier 	= 1.0;
	adsr->current_sample_index 		= 0;
	adsr->next_stage_sample_index 	= 0;
}

void
	env_note_on
(adsr_t *adsr)
{
	enter_stage
		(adsr
		 ,ATTACK);
}

void
	env_note_off
(adsr_t *adsr)
{
	enter_stage
		(adsr
		 ,RELEASE);
}

real
	env_get_sample
(adsr_t	*adsr)
{
	if (!(adsr->current_stage == OFF ||
				adsr->current_stage == SUSTAIN)
	   )
	{
		if (adsr->current_sample_index >= adsr->next_stage_sample_index)
		{
			stage_type new_stage = (adsr->current_stage + 1) % NUM_STAGES;
			enter_stage
				(adsr,
				 new_stage);
		}	

		adsr->current_level *= adsr->multiplier;
		adsr->current_sample_index += 1;
	}
	return adsr->current_level;
}
