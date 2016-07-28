#ifndef SYNTHCREATOR_H
#define SYNTHCREATOR_H

#include "Synth.h"

StereoSynth
createChordSynth( int samplerate )
{
	unsigned numVoices = 16;
	unsigned oscillatorsPerVoice = 3;

	Synth left(numVoices, oscillatorsPerVoice, samplerate);
    left.setCutoff(3000.0);
    left.setDetune( 10, 0);
    left.setDetune( 12, 1);
    left.setDetune( 14, 2);

	Synth right(numVoices, oscillatorsPerVoice, samplerate);
    right.setCutoff(3000.0);
    right.setDetune(-10, 0);
    right.setDetune(-12, 1);
    right.setDetune(-14, 2);

    return StereoSynth( left, right );
}


#endif
