#ifndef VOICE_H
#define VOICE_H

#include "Osc.h"
#include "Env.h"
#include "Filter.h"

#include <vector>

class Voice
{
public:

	Voice(unsigned numberOfOscillators, int samplerate);

	void reset();
	bool isActive();
	bool isPressed();
	
	void setMidiNote(int midiNote);
	int getMidiNote();

	void setFilterLfoMod(real filterLfoMod);

	void noteOn();
	void noteOff();

	real getSample();

	unsigned getNumberOfOscillators();

private:

	std::vector<Oscillator*>	a_oscillators;
	std::vector<Env*>			a_volume_envelopes;
	std::vector<Filter*>		a_filters;
	std::vector<Env*>			a_filter_envelopes;
	std::vector<real>			a_filter_envelope_amounts;
	real						filter_lfo_mod;

	int				midi_note;
	int				midi_velocity;

};

#endif