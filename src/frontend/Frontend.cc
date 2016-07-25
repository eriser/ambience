#include "Codec.h"
#include "Types.h"
#include "SynthWrapper.h"
#include "Representation.h"
#include "Evolution.h"
#include "Rules.h"
#include "Effect.h"
#include "Delay.h"

#include <cstdlib>

extern "C"
{
#include "synth.h"
}

#include <string.h>

std::vector<real>
individualToAudio(
	const std::string &       outputFile,
	ambience::Individual &	  individual,
	SynthWrapper &            synth,
	std::vector< Effect * > & fx,
	float                     bpm,
	unsigned                  slicesPerWholeNote,
	unsigned                  numberOfSlices,
	unsigned                  sliceLength
	)
{
	unsigned numberOfQuarters  = numberOfSlices / (slicesPerWholeNote / 4);
	float    quartersPerSecond = bpm / 60.0f;
	unsigned samplerate        = (unsigned) synth.getSampleRate();

	unsigned long numberOfSamples = (unsigned long) (samplerate * (1.0f / quartersPerSecond) * numberOfQuarters);
	unsigned long numberOfSamplesPerSlice = numberOfSamples / numberOfSlices;

	std::vector<real> audio(numberOfSamples, (real)0);

	unsigned long currentSample = 0;

	for (unsigned slice = 0; slice < numberOfSlices; slice++)
	{
		for (unsigned note = 0; note < sliceLength; note++)
		{
			ambience::Note currentNote = individual(slice, note, sliceLength).note();
			switch (currentNote)
			{
			case ambience::Note::ON:
				synth.noteOn(note);
				break;
			case ambience::Note::REST:
				synth.noteOff(note);
				break;
			default:
				break;
			}
		}

		for (unsigned long sample = 0; sample < numberOfSamplesPerSlice; sample++)
		{
			real audioSample = synth.getSample();
			for (Effect * effect : fx)
			{
				audioSample = effect->getSample(audioSample);
			}
			audio[currentSample + sample] = audioSample;

		}

		currentSample += numberOfSamplesPerSlice;
	}

	return audio;
}

int
main()
{

#ifndef NDEBUG
	std::cout << "--- Debug build ---" << std::endl;
#endif

    using namespace ambience;

	std::set<ambience::NoteValue> CMajor = {
		ambience::NoteValue::C,
		ambience::NoteValue::D,
		ambience::NoteValue::E,
		ambience::NoteValue::F,
		ambience::NoteValue::G,
		ambience::NoteValue::A,
		ambience::NoteValue::B,
	};

	std::set<ambience::NoteValue> CPentatonic = {
		ambience::NoteValue::C,
		ambience::NoteValue::D,
		ambience::NoteValue::E,
		ambience::NoteValue::G,
		ambience::NoteValue::A,
	};

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
    
	unsigned populationSize = 30;
	unsigned sliceLength = 128;
	unsigned numberOfSlices = 32;
	unsigned individualSize = sliceLength * numberOfSlices;
	bool verbose = true;
    GeneticAlgorithmRunner gar( populationSize, individualSize );



	ambience::NumberOfNotesEvaluator numberOfNotesEvaluator(6*5);
	gar.registerEvaluator(numberOfNotesEvaluator);
#if 0
	ambience::NoteTimeIntervalEvaluator notesInTimeIntervalEvaluator(4, 8, sliceLength);
	gar.registerEvaluator(notesInTimeIntervalEvaluator);
#endif
	ambience::NotesInRangeEvaluator notesInRangeEvaluator(40, 60, sliceLength);
	gar.registerEvaluator(notesInRangeEvaluator);
	ambience::HoldRestRatioEvaluator holdRestRatioEvaluator(0.3f);
	gar.registerEvaluator(holdRestRatioEvaluator);
	ambience::NotesInSetEvaluator notesInSetEvaluator(CPentatonic, sliceLength);
	gar.registerEvaluator(notesInSetEvaluator);
#if 0
	ambience::SingleNoteEvaluator singleNoteEvaluator(sliceLength, 1.0f);
	gar.registerEvaluator(singleNoteEvaluator);
#endif
	ambience::NotesInSliceEvaluator a(std::set<unsigned>({ 0, 4, 8, 12, 16, 20 }), sliceLength);
	gar.registerEvaluator(a);


    // gar.printPopulation();
    // std::cout << "Best Ind:" << std::endl;
    Individual best = gar.getBestIndividual();
    // best.print(sliceLength);
    // std::cout << "Fitness:" << std::endl;
    // std::cout << gar.evaluateIndividual( best ) << std::endl;

    gar.run(5000, 0.99999f, verbose); 
	

    // std::cout << "After run" << std::endl;
    // gar.printPopulation();
    // std::cout << "Best Ind:" << std::endl;
    best = gar.getBestIndividual();
    best.print(sliceLength);
    // std::cout << "Fitness:" << std::endl;
    // std::cout << gar.evaluateIndividual( best ) << std::endl;

#if 1
	int samplerate = 44100;
	SynthWrapper synth(samplerate);
	Delay delay(0.5f, 500, 0.5, true, 5000, samplerate);
	std::vector< Effect * > fx;
	fx.push_back( &delay );
	std::vector<real> audio = individualToAudio("tmp.wav", best, synth, fx, 120, 4, numberOfSlices, sliceLength);

#endif
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
