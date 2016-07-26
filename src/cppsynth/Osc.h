#ifndef OSC_H
#define OSC_H

#include "Types.h"

typedef enum
{
	SINE = 0,
	SAW,
	SQUARE,
	TRIANGLE,
	WHITE_NOISE
} waveform_type;

class Oscillator
{
public:

	Oscillator(int samplerate);

	void setFrequency(real frequency);
	void setDetune(real detune);

	real getSample();
	void reset();

private:

	void update_increment();
	real poly_blep(real t);

	waveform_type   waveform;
	real            frequency;
	real            phase;
	int             samplerate;
	real            gain;
	real            detune;
	real            last_output;
	real            phase_increment;

};

#endif