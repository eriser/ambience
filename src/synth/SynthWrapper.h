#ifndef SYNTHWRAPPER_H
#define SYNTHWRAPPER_H

extern "C"
{
#include "synth.h"
}

class SynthWrapper
{
public:

   SynthWrapper ( unsigned sampleRate );

   real
   getSample();

   void
   noteOn( int midiNote );

   void
   noteOff( int midiNote );

   void
   setParameter
   (synth_parameter_type      parameter,
    synth_parameter_union_t   value
    );

private:

   synth_t parameters_;

};

#endif
