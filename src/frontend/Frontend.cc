#include "Codec.h"
#include "Types.h"
#include "SynthWrapper.h"
#include "Representation.h"
#include "Evolution.h"
#include "Rules.h"

#include <cstdlib>

extern "C"
{
#include "synth.h"
}

#include <string.h>

void
individualToAudio(
	const std::string &    outputFile,
	ambience::Individual & individual,
	SynthWrapper &         synth,
	float                  bpm,
	unsigned               slicesPerWholeNote,
	unsigned               numberOfSlices,
	unsigned               sliceLength
	)
{
	unsigned numberOfQuarters  = numberOfSlices / (slicesPerWholeNote / 4);
	float    quartersPerSecond = bpm / 60.0f;
	unsigned samplerate        = (unsigned) synth.getSampleRate();

	unsigned long numberOfSamples = (unsigned long) (samplerate * (1.0f / quartersPerSecond) * numberOfQuarters);
	unsigned long numberOfSamplesPerSlice = numberOfSamples / numberOfSlices;

	real * audio = (real *) malloc(numberOfSamples * sizeof(real));
	memset(audio, 0, numberOfSamples * sizeof(real));

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
			audio[currentSample + sample] = synth.getSample();
		}

		currentSample += numberOfSamplesPerSlice;
	}

	writeWav(
		outputFile,
		&audio[0],
		numberOfSamples,
		1,
		samplerate,
		16
		);

	free(audio);
}

int
main()
{

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
    
	unsigned populationSize = 20;
	unsigned sliceLength = 128;
	unsigned numberOfSlices = 32;
	unsigned individualSize = sliceLength * numberOfSlices;
	bool verbose = true;
    GeneticAlgorithmRunner gar( 20, individualSize );

#if 1
	ambience::SingleNoteEvaluator singleNoteEvaluator(sliceLength);
	gar.registerEvaluator(singleNoteEvaluator);
	ambience::NumberOfNotesEvaluator numberOfNotesEvaluator(numberOfSlices / 8);
	gar.registerEvaluator(numberOfNotesEvaluator);
	ambience::NotesInSetEvaluator notesInSetEvaluator(CMajor, sliceLength);
	gar.registerEvaluator(notesInSetEvaluator);
	ambience::NotesInRangeEvaluator notesInRangeEvaluator(40, 80, sliceLength);
	gar.registerEvaluator(notesInRangeEvaluator);
	ambience::NoteTimeIntervalEvaluator notesInTimeIntervalEvaluator(6, 8, sliceLength);
	gar.registerEvaluator(notesInTimeIntervalEvaluator);
#endif

    // gar.printPopulation();
    // std::cout << "Best Ind:" << std::endl;
    Individual best = gar.getBestIndividual();
    // best.print(sliceLength);
    // std::cout << "Fitness:" << std::endl;
    // std::cout << gar.evaluateIndividual( best ) << std::endl;

    gar.run(8000, 0.999999f, verbose); 

    // std::cout << "After run" << std::endl;
    // gar.printPopulation();
    // std::cout << "Best Ind:" << std::endl;
    best = gar.getBestIndividual();
    // best.print(sliceLength);
    // std::cout << "Fitness:" << std::endl;
    // std::cout << gar.evaluateIndividual( best ) << std::endl;

#if 1
	int samplerate = 44100;
	SynthWrapper synth(samplerate);
	individualToAudio("tmp.wav", best, synth, 120, 16, numberOfSlices, sliceLength);
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
