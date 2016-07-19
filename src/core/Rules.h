#ifndef RULES_H
#define RULES_H

#include "Representation.h"

#include <set>

namespace ambience
{

// Best fitness for a large number of rests
class RestEvaluator : public ambience::Evaluator
{
public:
    virtual float evaluate( const ambience::Individual & individual )
    {
        int numberOfRests = 0;
        for ( unsigned i = 0; i < individual.size(); i++ )
        {
            if ( individual[i].note() == ambience::Note::REST )
            {
                numberOfRests++;
            }
        }
        return (float) numberOfRests / (float) individual.size();
    }
};

// Best fitness if <= 1 notes per slice 
class SingleNoteEvaluator : public ambience::Evaluator
{
public:

	SingleNoteEvaluator(unsigned sliceLength) :
		sliceLength_(sliceLength)
	{}

	virtual float evaluate(const ambience::Individual & individual)
	{
		float fitness = 0;
		unsigned numberOfSlices = individual.numberOfSlices(sliceLength_);
		for (unsigned slice = 0; slice < numberOfSlices; slice++)
		{
			unsigned numberOfRealNotes = individual.count(ambience::Note::ON, slice, sliceLength_);

			if (numberOfRealNotes <= 1)
			{
				fitness += 1.0;
			}
			else
			{
				fitness += 1.0f / (float)numberOfRealNotes;
			}
		}

		return fitness / (float)numberOfSlices;
	}

private:

	unsigned sliceLength_;

};

// Best fitness if the overall number of notes in the individual
// is near / equal the passed number of notes.
class NumberOfNotesEvaluator : public ambience::Evaluator
{
public:

	NumberOfNotesEvaluator(unsigned desiredNumberOfNotes) :
		desiredNumberOfNotes_(desiredNumberOfNotes)
	{}

	virtual float evaluate(const ambience::Individual & individual)
	{
		unsigned actualNumberOfNotes = individual.count(ambience::Note::ON);
		float fitness = 1.0f - fabs((float)((float)desiredNumberOfNotes_ - (float)actualNumberOfNotes)) / (float)individual.size();
		return fitness;
	}

private:

	unsigned desiredNumberOfNotes_;

};

// Best fitness if all notes in the individual are in the passed set
class NotesInSetEvaluator : public ambience::Evaluator
{
public:

	NotesInSetEvaluator(std::set<ambience::NoteValue> & desiredNotes, unsigned sliceLength) :
		desiredNotes_(desiredNotes), sliceLength_(sliceLength)
	{}

	virtual float evaluate(const ambience::Individual & individual)
	{
		unsigned numberOfNotes = 0;
		unsigned numberOfNotesInSet    = 0;
		for (unsigned i = 0; i < individual.size(); i++)
		{ 
			if (individual[i].note() == ambience::Note::ON)
			{
				unsigned midiNote = i % sliceLength_;
				const bool noteInSet = desiredNotes_.find(midiToNoteValue(midiNote)) != desiredNotes_.end();
				if (noteInSet)
				{
					numberOfNotesInSet++;
				}
				numberOfNotes++;
			}
		}
		
		return (float)numberOfNotesInSet / (float)numberOfNotes;
	}

private:

	std::set<ambience::NoteValue> desiredNotes_;
	unsigned sliceLength_;
};

// Best fitness if all notes are in the passed range
class NotesInRangeEvaluator : public ambience::Evaluator
{
public:

    NotesInRangeEvaluator( unsigned minNote, unsigned maxNote, unsigned sliceLength ) :
        minNote_( minNote ), maxNote_( maxNote ), sliceLength_( sliceLength )
    {}

    virtual float evaluate( const ambience::Individual & individual )
    {
       unsigned numSlotsInRange    = maxNote_ - minNote_ + 1;
       unsigned numSlotsNotInRange = sliceLength_ - numSlotsInRange;

       unsigned notesOutOfRange = 0;       

       for ( unsigned slice = 0; slice < individual.numberOfSlices( sliceLength_ ); slice++ )
       {
          for ( unsigned note = 0; note < sliceLength_; note++ )
          {
             if ( note < minNote_ || note > maxNote_ )
             {
                if ( individual( slice, note, sliceLength_ ).note() == ambience::Note::ON )   
                {
                   notesOutOfRange++;
                }
             }  
          }
       }
       
       float avgNumNotesOutOfRange = (float) notesOutOfRange / (float) individual.numberOfSlices( sliceLength_ );
       float fitness = 1.0f - avgNumNotesOutOfRange / (float) numSlotsNotInRange;
        
       return fitness;
    }

private:

    unsigned minNote_;
    unsigned maxNote_;
    unsigned sliceLength_;

};

// Best fitness if there are no notes in slices before and after a slice with notes
// as specified by the parameters
// interval == 0 means notes follow directly, 
// interval == 1 means one slice of hold / rest
// ...
// The interval from start to first note does not count
class NoteTimeIntervalEvaluator : public Evaluator
{
public:
   NoteTimeIntervalEvaluator( unsigned minEmptySlices, unsigned maxEmptySlices, 
                              unsigned sliceLength  ) :
      minEmptySlices_( minEmptySlices ), maxEmptySlices_( maxEmptySlices ), 
      sliceLength_( sliceLength )
   {}

   virtual float evaluate( const Individual & individual )
   {
      unsigned numberOfSlices = individual.numberOfSlices( sliceLength_ );
      std::vector< unsigned > intervalLength;
      bool firstNotePassed = false;
      for (unsigned slice = 0; slice < numberOfSlices; slice++)
      {
         unsigned numberOfRealNotes = individual.count(ambience::Note::ON, slice, sliceLength_);
         if ( numberOfRealNotes > 0 )
         { 
            intervalLength.push_back( 0 );
            firstNotePassed = true;
         }
         else if ( numberOfRealNotes == 0 && firstNotePassed )
         {
            intervalLength.back()++;
         }
      }

      float fitness = 0.0f;
      for ( unsigned interval : intervalLength )
      {
         if ( interval < minEmptySlices_ || interval > maxEmptySlices_ )
         {
            fitness += 1.0f;
         }
      }

      fitness = 1.0f - fitness / (float) intervalLength.size();
      return fitness;

   }

private:

    unsigned minEmptySlices_;
    unsigned maxEmptySlices_;
    unsigned sliceLength_;

};

}

#endif
