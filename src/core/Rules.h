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

// Best fitness if hold to rest ratio fits the passed value
// fitness = | ratio - (rests / (holds + rests)) |
// => ratio = 0: full hold
// => ratio = 1: full rest
class HoldRestRatioEvaluator : public ambience::Evaluator
{
public:

	HoldRestRatioEvaluator( float holdToRestRatio ) :
		holdToRestRatio_( holdToRestRatio )
	{}

	virtual float evaluate(const ambience::Individual & individual)
	{
		unsigned numberOfRests = individual.count(ambience::Note::REST);
		unsigned numberOfHolds = individual.count(ambience::Note::HOLD);

		float restPercentage = (float)numberOfRests / (float)(numberOfRests + numberOfHolds);

		return 1.0f - fabs(holdToRestRatio_ - restPercentage);
	}

private:

	float holdToRestRatio_;
};

// Best fitness if <= 1 notes per slice 
class SingleNoteEvaluator : public ambience::Evaluator
{
public:

	SingleNoteEvaluator(unsigned sliceLength, float emptySliceFitness = 0.5f) :
		sliceLength_(sliceLength), emptySliceFitness_( emptySliceFitness )
	{}

	virtual float evaluate(const ambience::Individual & individual)
	{
		float fitness = 0;
		unsigned numberOfSlices = individual.numberOfSlices(sliceLength_);
		for (unsigned slice = 0; slice < numberOfSlices; slice++)
		{
			unsigned numberOfRealNotes = individual.count(ambience::Note::ON, slice, sliceLength_);

			if (numberOfRealNotes == 1)
			{
				fitness += 1.0;
			}
			else if (numberOfRealNotes == 0)
			{
				fitness += emptySliceFitness_;
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
	float emptySliceFitness_;

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
	   unsigned notesInRange = 0;
	   unsigned numberOfNotes = individual.count(ambience::Note::ON);

       for ( unsigned slice = 0; slice < individual.numberOfSlices( sliceLength_ ); slice++ )
       {
          for ( unsigned note = minNote_; note <= maxNote_; note++ )
          {
             if ( individual( slice, note, sliceLength_ ).note() == ambience::Note::ON )   
             {
                notesInRange++;
             }
          }
       }
        
       return (float) notesInRange / (float) numberOfNotes;
    }

private:

    unsigned minNote_;
    unsigned maxNote_;
    unsigned sliceLength_;

};

#if 0
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
#endif

// Best fitness if notes are only present in the specified slices
class NotesInSliceEvaluator : public Evaluator
{
public:

	NotesInSliceEvaluator(std::set< unsigned > & noteSlices, unsigned sliceLength) :
		noteSlices_(noteSlices), sliceLength_(sliceLength)
	{}

	virtual float evaluate(const Individual & individual)
	{
		float fitness = 0.0f;
		unsigned numberOfNoteSlices = 0;
		unsigned numberOfSlices = individual.numberOfSlices(sliceLength_);
		for (unsigned slice = 0; slice < numberOfSlices; slice++)
		{
			if (noteSlices_.find(slice) == noteSlices_.end())
			{
				unsigned numNotesInSlice = individual.count(ambience::Note::ON, slice, sliceLength_);
				fitness += (float)numNotesInSlice / (float)sliceLength_;
				numberOfNoteSlices++;
			}
		}

		return 1.0f - (fitness / (float)numberOfNoteSlices);
	}

private:

	std::set< unsigned > noteSlices_;
	unsigned sliceLength_;
};

}

#endif
