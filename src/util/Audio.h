#ifndef AUDIO_H
#define AUDIO_H

#include "Types.h"

#include <vector>
#include <cassert>

std::vector<real>
interleave( std::vector<real> & left, std::vector<real> & right )
{
    assert( left.size() == right.size() );
	std::vector<real> interleaved(left.size() + right.size(), (real)0);
	for (unsigned i = 0; i < left.size(); i++)
	{
		interleaved[2 * i] = left[i];
		interleaved[2 * i + 1] = right[i];
	}

    return interleaved;

}

#endif
