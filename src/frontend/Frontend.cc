#include "Codec.h"
#include "Types.h"
#include "SynthWrapper.h"


int
main()
{

   SynthWrapper synth( 44100 );

   real audio[44100];

   synth.noteOn( 60 );
   for ( int i = 0; i < 44100; i++ )
   {
      audio[i] = synth.getSample();
   }
   synth.noteOff( 60 );

   writeWav(
         "/tmp/test.wav",
         &audio[0],
         16,
         1,
         44100 / 2,
         16
   );

   return 0;
}
