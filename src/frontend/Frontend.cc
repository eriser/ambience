#include "Codec.h"
#include "Types.h"
#include "SynthWrapper.h"
#include "Representation.h"
#include "Evolution.h"
#include "Rules.h"

extern "C"
{
#include "synth.h"
}

#include <string.h>

int
main()
{

    using namespace ambience;
#if 0
    Chromosome c;
    c.print();
    std::cout << "Number of rests: " << c.count(Note::REST) << std::endl;

    Individual i(4);
    i.print();
    std::cout << std::endl;
    i.mutate();
    i.print();
    std::cout << std::endl;
    i.mutate();
    i.print();


    Population p( 5, 2 );
    // p.print();
#endif

#if 1
    
	unsigned populationSize = 10;
	unsigned sliceLength = 64;
	unsigned numberOfSlices = 32;
	unsigned individualSize = sliceLength * numberOfSlices;
	bool verbose = true;
    GeneticAlgorithmRunner gar( 20, individualSize );
#if 0
	ambience::RestEvaluator restEvaluator;
	ambience::CEvaluator cEvaluator;
    gar.registerEvaluator( restEvaluator );
    gar.registerEvaluator( cEvaluator );
#endif
	ambience::SingleNoteEvaluator singleNoteEvaluator(sliceLength);
	gar.registerEvaluator(singleNoteEvaluator);

    // gar.printPopulation();
    std::cout << "Best Ind:" << std::endl;
    Individual best = gar.getBestIndividual();
    best.print(sliceLength);
    std::cout << "Fitness:" << std::endl;
    std::cout << gar.evaluateIndividual( best ) << std::endl;

    gar.run(8000, verbose); 

    std::cout << "After run" << std::endl;
    // gar.printPopulation();
    std::cout << "Best Ind:" << std::endl;
    best = gar.getBestIndividual();
    best.print(sliceLength);
    std::cout << "Fitness:" << std::endl;
    std::cout << gar.evaluateIndividual( best ) << std::endl;
#endif
    

#if 0
   int numSamples = 88200;

   SynthWrapper synth( 44100 );

   real audio[numSamples];
   memset ( audio, 0, numSamples * sizeof(real) );

   synth_parameter_union value;

   synth.noteOn( 69 );
   value.real_value = .1;
   synth.setParameter( OSC_ALL_CUTOFF, value );

   for ( int i = 0; i < numSamples; i++ )
   {
      audio[i] = synth.getSample();
   }
   synth.noteOff( 69 );

   writeWav(
         "/tmp/test.wav",
         &audio[0],
         numSamples,
         1,
         44100,
         16
   );
#endif
   return 0;
}
