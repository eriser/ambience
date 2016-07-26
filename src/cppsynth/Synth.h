#ifndef SYNTH_H
#define SYNTH_H

#include "Voice.h"

#include <vector>

class Synth
{
public:

	Synth(unsigned numVoices, unsigned oscillatorsPerVoice, int samplerate);

	void noteOn(int midiNote);
	void noteOff(int midiNote);

	real getSample();

	int getSampleRate();

private:

	int getFreeVoiceIndex();

	int						samplerate;
	std::vector<Voice*>     a_voices;
	Oscillator				lfo;
	real					lfo_to_volume;
	real					lfo_to_filter;

};

#endif