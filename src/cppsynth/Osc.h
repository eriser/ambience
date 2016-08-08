#ifndef OSC_H
#define OSC_H

#include <string>

#include "Types.h"

typedef enum
{
	SINE = 0,
	SAW,
	SQUARE,
	TRIANGLE,
	WHITE_NOISE
} waveform_type;

std::string waveFormToString(waveform_type waveform);

class Oscillator
{
public:

	Oscillator(int samplerate);

	void setFrequency(real frequency);
	void setDetune(real detune);
	void setWaveform(waveform_type waveform) { this->waveform_ = waveform; };

	real getDetune() { return detune_;  }
	waveform_type getWaveform() { return waveform_; }

	real getSample();
	void reset();

private:

    void updateFrequency();
	void update_increment();
	real poly_blep(real t);

	waveform_type   waveform_;
	real            frequency_;
    real            dryFrequency_;
	real            phase;
	int             samplerate;
	real            gain;
	real            detune_;
	real            last_output;
	real            phase_increment;

};

#endif
