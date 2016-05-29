/* filter.c */

#include <string.h>
#include <stdlib.h>

#include "types.h"
#include "filter.h"

static
	real
calculate_cutoff(filter_t *filter)
{
	real val = filter->cutoff + filter->cutoff_mod;
	val = val < 0.0 ? 0.0 : val;
	val = val > 0.999 ? 0.999 : val;
	return val;
}

static
inline
	void
calculate_feedback_amount(filter_t *filter)
{
	filter->feedback_amount = filter->resonance + filter->resonance / (1.0 - calculate_cutoff(filter));
}


void
	filter_init
(filter_t	*filter)
{
	memset(filter->a_buffer, 0, sizeof(filter->a_buffer));

	filter->cutoff = 0.5;
	filter->resonance = 0.0;
	filter->cutoff_mod = 0.0;

	calculate_feedback_amount(filter);

	filter->mode = LOWPASS;
	filter->steepness = MINUS_24_DB;
}

void
	filter_reset
(filter_t	*filter)
{
	memset(filter->a_buffer, 0, sizeof(filter->a_buffer));
}

void
	filter_set_cutoff
(filter_t *filter,
 real cutoff)
{
	filter->cutoff = cutoff;
	calculate_feedback_amount(filter);
}

void
	filter_set_resonance
(filter_t *filter,
 real resonance)
{
	filter->resonance = resonance;
	calculate_feedback_amount(filter);
}


void
	filter_set_cutoff_mod
(filter_t *filter,
 real cutoff_mod)
{
	filter->cutoff_mod = cutoff_mod;
	calculate_feedback_amount(filter);
}


real
	filter_get_sample
(filter_t *filter,
 real input)
{
	if (input == 0.0)
	{
		return 0.0;
	}

	real cutoff = calculate_cutoff(filter);

	filter->a_buffer[0] += cutoff * (input - filter->a_buffer[0] + filter->feedback_amount * (filter->a_buffer[0] - filter->a_buffer[1]));
	filter->a_buffer[1] += cutoff * (filter->a_buffer[0] - filter->a_buffer[1]);

	if (filter->steepness == MINUS_12_DB)
	{
		if (filter->mode == LOWPASS)
		{
			return filter->a_buffer[1];
		}
		else if (filter->mode == BANDPASS)
		{
			return filter->a_buffer[0] - filter->a_buffer[1];
		}
		else if (filter->mode == HIGHPASS)
		{
			return input - filter->a_buffer[0];
		}

	}

	else if (filter->steepness == MINUS_24_DB)
	{
		filter->a_buffer[2] += cutoff * (filter->a_buffer[1] - filter->a_buffer[2]);
		filter->a_buffer[3] += cutoff * (filter->a_buffer[2] - filter->a_buffer[3]);

		if (filter->mode == LOWPASS)
		{
			return filter->a_buffer[3];
		}
		else if (filter->mode == BANDPASS)
		{
			return filter->a_buffer[0] - filter->a_buffer[3];
		}
		else if (filter->mode == HIGHPASS)
		{
			return input - filter->a_buffer[3];
		}
	}

	return input;

}










