/* filter.h */

#ifndef FILTER_H
#define FILTER_H

#include "types.h"

#define FILTER_BUFFER_LENGTH (4)

typedef enum
{
	LOWPASS = 0,
	BANDPASS,
	HIGHPASS,
	NUM_MODES,
} filter_mode_type;

typedef enum
{
	MINUS_12_DB = 0,
	MINUS_24_DB,
	NUM_STEEPNESSES
} filter_steepness_type;

typedef struct filter_s
{
	real	a_buffer[FILTER_BUFFER_LENGTH];
	real	cutoff;
	real	resonance;
	real	cutoff_mod;
	real	feedback_amount;
	filter_mode_type		mode;
	filter_steepness_type 	steepness;
} filter_t;

real
filter_get_sample
(filter_t *filter,
 real input);


void
filter_set_cutoff_mod
(filter_t *filter,
 real cutoff_mod);

void
filter_set_resonance
(filter_t *filter,
 real resonance);

void
filter_set_cutoff
(filter_t *filter,
 real cutoff);

void
filter_reset
(filter_t   *filter);

void
filter_init
(filter_t   *filter);



#endif
