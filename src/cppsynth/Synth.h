#ifndef SYNTH_H
#define SYNTH_H

#include "Voice.h"

#include <vector>

class Voice;

class Synth
{
public:

	Synth(unsigned numVoices, unsigned oscillatorsPerVoice, int samplerate);

	void reset();

	void noteOn(int midiNote);
	void noteOff(int midiNote);

	real getSample();

	int getSampleRate();
	unsigned getOscillatorsPerVoice() { return oscillatorsPerVoice; }

	void printParameters();
	void printParameters(const std::string & name);

	// setters

    void setCutoff(real frequency);
	void setCutoff(real frequency, int oscillatorId);

    void setDetune(real cents, int oscillatorId);

	void setAttack(real attack);
	void setAttack(real attack, int oscillatorId);

	void setDecay(real decay);
	void setDecay(real decay, int oscillatorId);

	void setSustain(real sustain);
	void setSustain(real sustain, int oscillatorId);

	void setRelease(real release);
	void setRelease(real release, int oscillatorId);

	void setFilterEnvelopeAmount(real amount);
	void setFilterEnvelopeAmount(real amount, int oscillatorId);

	// getters

	real getCutoff(int oscillatorId);

	real getDetune(int oscillatorId);

	real getAttack(int oscillatorId);
	real getDecay(int oscillatorId);
	real getSustain(int oscillatorId);
	real getRelease(int oscillatorId);

	real getFilterEnvelopeAmount(int oscillatorId);

private:

	int getFreeVoiceIndex();

	int						samplerate;
	unsigned                oscillatorsPerVoice;
	std::vector<Voice*>     a_voices;
	Oscillator				lfo;
	real					lfo_to_volume;
	real					lfo_to_filter;

};

struct StereoSynth
{
    StereoSynth( Synth & l, Synth & r ) :
        left( l ), right( r )
    {}

	StereoSynth(unsigned numVoices, unsigned oscillatorsPerVoice, int samplerate) :
		left(numVoices, oscillatorsPerVoice, samplerate),
		right(numVoices, oscillatorsPerVoice, samplerate)
	{}

    Synth left;
    Synth right;
};

#endif
