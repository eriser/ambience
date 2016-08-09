#ifndef INDIVIDUALCREATOR_H
#define INDIVIDUALCREATOR_H

#include "Representation.h"
#include "Evolution.h"
#include "Rules.h"

ambience::Individual
createMelody( unsigned sliceLength, unsigned numberOfSlices, unsigned numberOfNotes,
              std::set<ambience::NoteValue> noteSet )
{
    using namespace ambience;

	unsigned populationSize = 30;
	unsigned individualSize = sliceLength * numberOfSlices;
	bool verbose = true;

    GeneticAlgorithmRunner gar( populationSize, numberOfSlices, sliceLength );

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

ambience::Individual
createChords( unsigned sliceLength, unsigned numberOfSlices, unsigned numberOfNotes,
              std::set<ambience::NoteValue> noteSet, std::set<unsigned> chordSlicePositions )
{
    using namespace ambience;

	unsigned populationSize = 30;
	unsigned individualSize = sliceLength * numberOfSlices;
	bool verbose = true;

    GeneticAlgorithmRunner gar( populationSize, numberOfSlices, sliceLength );

	NumberOfNotesEvaluator numberOfNotesEvaluator(6*5);
	gar.registerEvaluator(numberOfNotesEvaluator);

    NotesInRangeEvaluator notesInRangeEvaluator(50, 70, sliceLength);
	gar.registerEvaluator(notesInRangeEvaluator);

	HoldRestRatioEvaluator holdRestRatioEvaluator(0.3f);
	gar.registerEvaluator(holdRestRatioEvaluator);

	NotesInSetEvaluator notesInSetEvaluator(noteSet, sliceLength);
	gar.registerEvaluator(notesInSetEvaluator);

	NotesInSliceEvaluator notesInSliceEvaluator(chordSlicePositions, sliceLength);
	gar.registerEvaluator(notesInSliceEvaluator);

    Individual best = gar.getBestIndividual();

    gar.run(5000, 0.99999f, verbose); 

    return gar.getBestIndividual();
}
#endif
