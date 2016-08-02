#ifndef SYNTHCREATOR_H
#define SYNTHCREATOR_H

#include "Synth.h"
#include "Types.h"
#include "ConvenienceMath.h"

void
toChordSynth(StereoSynth & synth, bool punchy = true)
{

	using namespace ambience;

	// synth.left.reset();
	// synth.right.reset();

	real attack = punchy ? util::randomRealInterval(0.1, 0.6) : util::randomRealInterval(1.0, 2.0);
	real decay = punchy ? util::randomRealInterval(0.1, 0.6) : util::randomRealInterval(1.0, 2.0);
	real sustain = 0.7;
	real release = util::randomRealInterval(5.0, 8.0);

	real cutoff = util::randomRealInterval(500.0, 1500.0);

	int detuneOffset = util::randomIntInterval(0, 5);

	real filterEnvelopeAmount = punchy ? util::randomRealInterval(0.8, 0.9) : 0.0;


	// left

	synth.left.setAttack(attack);
	synth.left.setDecay(decay);
	synth.left.setSustain(sustain);
	synth.left.setRelease(release);

	synth.left.setCutoff(cutoff);

	for (unsigned i = 0; i < synth.left.getOscillatorsPerVoice(); i++)
	{
		synth.left.setDetune(0.0 + i * detuneOffset, i);
	}

	synth.left.setFilterEnvelopeAmount(filterEnvelopeAmount);

	// right

	synth.right.setAttack(attack);
	synth.right.setDecay(decay);
	synth.right.setSustain(sustain);
	synth.right.setRelease(release);

	synth.right.setCutoff(cutoff);

	for (unsigned i = 0; i < synth.right.getOscillatorsPerVoice(); i++)
	{
		synth.right.setDetune(0.0 - i * detuneOffset, i);
	}

	synth.right.setFilterEnvelopeAmount(filterEnvelopeAmount);

}

StereoSynth
createChordSynth( int samplerate )
{
	unsigned numVoices = 16;
	unsigned oscillatorsPerVoice = 3;

	Synth left(numVoices, oscillatorsPerVoice, samplerate);
    left.setCutoff(1000.0);
    left.setDetune( 10, 0);
    left.setDetune( 12, 1);
    left.setDetune( 14, 2);

	Synth right(numVoices, oscillatorsPerVoice, samplerate);
    right.setCutoff(1000.0);
    right.setDetune(-10, 0);
    right.setDetune(-12, 1);
    right.setDetune(-14, 2);

    return StereoSynth( left, right );
}


#endif
