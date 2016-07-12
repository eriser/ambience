#ifndef RULES_H
#define RULES_H

#include "Representation.h"

#include <set>

namespace ambience
{

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

}

#endif
