/* env.h */

#ifndef ENV_H
#define ENV_H

#include "types.h"

#define ENV_MIN_LEVEL (0.0001)

typedef enum
{
	OFF = 0,
	ATTACK,
	DECAY,
	SUSTAIN,
	RELEASE,
	NUM_STAGES
} stage_type;

typedef struct adsr_s
{
	real		a_stage_value[NUM_STAGES];
	real 		min_level;
	stage_type	current_stage;
	real		current_level;
	real		multiplier;
	int			samplerate;
	unsigned long long			current_sample_index;
	unsigned long long			next_stage_sample_index;
} adsr_t;

void
env_init
(adsr_t	*adsr
 ,int	 samplerate
 );

void
env_reset
(adsr_t *adsr);

void
env_note_on
(adsr_t *adsr);

void
env_note_off
(adsr_t *adsr);

real
env_get_sample
(adsr_t *adsr);





#endif
