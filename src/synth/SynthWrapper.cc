#include "SynthWrapper.h"

extern "C"
{
#include "synth.h"
}

SynthWrapper::SynthWrapper( unsigned samplerate )
{
   synth_init( &parameters_, (int) samplerate );
}


real
SynthWrapper::getSample()
{
   return synth_get_sample( &parameters_ );
}


void
SynthWrapper::noteOn( int midiNote )
{
   synth_note_on( &parameters_, midiNote );
}


void
SynthWrapper::noteOff( int midiNote )
{
   synth_note_off( &parameters_, midiNote );
}


void
SynthWrapper::setParameter
(synth_parameter_type      parameter,
 synth_parameter_union_t   value
 )
{
   synth_set_parameter( &parameters_, parameter, value );
}
