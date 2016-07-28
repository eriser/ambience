#include "Codec.h"
#include "Types.h"
// #include "SynthWrapper.h"
#include "Representation.h"
#include "Evolution.h"
#include "Rules.h"
#include "Effect.h"
#include "Delay.h"
#include "Synth.h"
#include "Audio.h"
#include "SynthCreator.h"

#include <cstdlib>
#include <string.h>

#if 0
extern "C"
{
#include "synth.h"
}
#endif

std::vector<real>
individualToAudio(
	ambience::Individual &	  individual,
	Synth &                   synth,
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


ambience::Individual
createMelody( unsigned sliceLength, unsigned numberOfSlices, unsigned numberOfNotes,
              std::set<ambience::NoteValue> noteSet )
{
    using namespace ambience;

	unsigned populationSize = 30;
	unsigned individualSize = sliceLength * numberOfSlices;
	bool verbose = true;

    GeneticAlgorithmRunner gar( populationSize, individualSize );

	NumberOfNotesEvaluator numberOfNotesEvaluator(numberOfNotes);
	gar.registerEvaluator(numberOfNotesEvaluator);
	
    NotesInRangeEvaluator notesInRangeEvaluator(50, 70, sliceLength);
	gar.registerEvaluator(notesInRangeEvaluator);

	HoldRestRatioEvaluator holdRestRatioEvaluator(0.3f);
	gar.registerEvaluator(holdRestRatioEvaluator);
	
    ambience::NotesInSetEvaluator notesInSetEvaluator(noteSet, sliceLength);
	gar.registerEvaluator(notesInSetEvaluator);

	ambience::SingleNoteEvaluator singleNoteEvaluator(sliceLength, 1.0f);
	gar.registerEvaluator(singleNoteEvaluator);

#if 0
	std::set<unsigned> set;
	ambience::NotesInSliceEvaluator a(set, sliceLength);
	gar.registerEvaluator(a);
#endif

    gar.run(5000, 0.99999f, verbose); 

    return gar.getBestIndividual();
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
	ambience::NotesInRangeEvaluator notesInRangeEvaluator(50, 70, sliceLength);
	gar.registerEvaluator(notesInRangeEvaluator);
	ambience::HoldRestRatioEvaluator holdRestRatioEvaluator(0.3f);
	gar.registerEvaluator(holdRestRatioEvaluator);
	ambience::NotesInSetEvaluator notesInSetEvaluator(CPentatonic, sliceLength);
	gar.registerEvaluator(notesInSetEvaluator);
#if 0
	ambience::SingleNoteEvaluator singleNoteEvaluator(sliceLength, 1.0f);
	gar.registerEvaluator(singleNoteEvaluator);
#endif
	std::set<unsigned> set = { 0, 4, 8, 12, 16, 20 };
	ambience::NotesInSliceEvaluator a(set, sliceLength);
	gar.registerEvaluator(a);

    Individual best = gar.getBestIndividual();

    gar.run(5000, 0.99999f, verbose); 

    best = gar.getBestIndividual();
    // best.print(sliceLength);


	GeneticAlgorithmRunner gar2(populationSize, individualSize);

	gar2.registerEvaluator(numberOfNotesEvaluator);
	gar2.registerEvaluator(notesInRangeEvaluator);
	gar2.registerEvaluator(holdRestRatioEvaluator);
	gar2.registerEvaluator(notesInSetEvaluator);
	gar2.registerEvaluator(a);

	Individual best2 = gar2.getBestIndividual();

	// gar2.run(5000, 0.99999f, verbose);

	best2 = gar2.getBestIndividual();
	// best2.print(sliceLength);

    Individual melody = createMelody( sliceLength, numberOfSlices, numberOfSlices, CPentatonic );

#if 1
	unsigned numVoices = 16;
	unsigned oscillatorsPerVoice = 3;
	int samplerate = 44100;
    unsigned slicesPerWholeNote = 4;

    StereoSynth chordSynth = createChordSynth( samplerate );

	Delay delay(0.5f, 500, 0.5, true, 5000, samplerate);
	std::vector< Effect * > fx;
	fx.push_back( &delay );
	std::vector<real> audioLeft = individualToAudio(best, chordSynth.left, fx, 120, slicesPerWholeNote, numberOfSlices, sliceLength);

	Delay delay2(0.5f, 500, 0.5, true, 5000, samplerate);
	std::vector< Effect * > fx2;
	fx2.push_back(&delay2);
	std::vector<real> audioRight = individualToAudio(best, chordSynth.right, fx2, 120, slicesPerWholeNote, numberOfSlices, sliceLength);


	std::vector<real> audioInterleaved = interleave( audioLeft, audioRight );

    
	Synth melodySynth(numVoices, oscillatorsPerVoice, samplerate);
    melodySynth.setCutoff(5000.0);
    melodySynth.setDetune( 10, 0);
    melodySynth.setDetune( 10, 1);
    melodySynth.setDetune( 10, 2);
	Delay melodyDelay(0.5f, 500, 0.5, true, 5000, samplerate);
	std::vector< Effect * > melodyFx;
	fx.push_back( &melodyDelay );
	std::vector<real> audioMelody = individualToAudio(melody, melodySynth, melodyFx, 120, slicesPerWholeNote, numberOfSlices, sliceLength);

    std::vector<real> audioMelodyInterleaved = interleave( audioMelody, audioMelody );

    std::vector<real> audioAll = add( audioMelodyInterleaved, audioInterleaved );
	writeWav("tmp.wav", audioAll, 2, samplerate, 16);
	// writeWav("tmp.wav", audioMelodyInterleaved, 2, samplerate, 16);

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
