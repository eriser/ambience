#ifndef EFFECT_H
#define EFFECT_H

#include "Types.h"

class Effect
{
public:

	virtual ~Effect() {}
	virtual real getSample(real inputSample) = 0;
};

#endif