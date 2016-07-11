#ifndef RULES_H
#define RULES_H

#include "Representation.h"

namespace ambience
{

class RestEvaluator : public ambience::Evaluator
{
public:
    virtual float evaluate( const ambience::Individual & individual )
    {
        int numberOfRests = 0;
        for ( int i = 0; i < individual.size(); i++ )
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

}

#endif
