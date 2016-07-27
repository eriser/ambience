#ifndef SYNTH_H
#define SYNTH_H

#include "Voice.h"

#include <vector>

class Voice;

class Synth
{
public:

	Synth(int numVoices, int oscillatorsPerVoice, int samplerate);

	void noteOn(int midiNote);
	void noteOff(int midiNote);

	real getSample();

	int getSampleRate();

    void setCutoff(real frequency);
    void setDetune(real cents, int oscillatorId);

private:

	int getFreeVoiceIndex();

	int						samplerate;
	std::vector<Voice*>     a_voices;
	Oscillator				lfo;
	real					lfo_to_volume;
	real					lfo_to_filter;

};

#endif
