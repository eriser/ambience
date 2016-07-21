#ifndef REPRESENTATION_H
#define REPRESENTATION_H

#include <array>
#include <vector>
#include <map>
#include <iostream>
#include <cassert>
#include <typeinfo>

namespace ambience
{

enum class Note
{
    ON,
    REST,
    HOLD,
    NUMBER_OF_NOTES
};

enum class NoteValue
{
	C,
	Db,
	D,
	Eb,
	E,
	F,
	Gb,
	G,
	Ab,
	A,
	Bb,
	B,
	NUMBER_OF_NOTE_VALUES
};

const std::map< Note, std::string > 
noteToString = {
    { Note::ON,   "ON" },
    { Note::REST, "REST" },
    { Note::HOLD, "HOLD" }
};

NoteValue
midiToNoteValue(unsigned midiNote)
{
	return static_cast<NoteValue>(midiNote % static_cast<unsigned>(NoteValue::NUMBER_OF_NOTE_VALUES));
}

class Individual;
class Chromosome;
class Population;


class Evaluator
{
public:
    virtual ~Evaluator(){}
    virtual float evaluate( const Individual & individual ) = 0;
	virtual std::string getName()
	{
		return typeid(*this).name();
	}
};


class Chromosome
{
public:

    Chromosome()
    {
        shuffle();
    }

    Chromosome( Note note ) :
        note_( note )
    {}

    Note
    note() const
    {
        return note_;
    }

#if 0
    int
    count( Note note ) const
    {
        int numberOfPassedNote = 0;
        for ( int i = 0; i < notes_.size(); i++ )
        {
            if ( notes_[i] == note )
            {
                numberOfPassedNote++;
            }
        }
        return numberOfPassedNote;
    }
#endif
    void 
    shuffle()
    {
        int numberOfNotes = static_cast< int >( Note::NUMBER_OF_NOTES );
        note_ = static_cast< ambience::Note >( rand() % numberOfNotes ); 
    }

    void
    print()
    {
        std::cout << noteToString.at(note_) << " ";
    }

private:
#if 0
    const static int CHROMOSOME_LENGTH = 30;
    std::array< Note, CHROMOSOME_LENGTH > notes_;
#endif
    Note note_;
};


class Individual
{
public:
    
    Individual()
    {}

    Individual( int length )
    {
        for ( int i = 0; i < length; i++ )
        {
            Chromosome chromosome;
            chromosomes_.push_back( chromosome );
        }
    }

    Chromosome &
    operator[]( int i )
    {
        return chromosomes_[i];
    }

    const
    Chromosome &
    operator[]( int i ) const
    {
        return chromosomes_[i];
    }

    unsigned
    size() const
    {
        return chromosomes_.size();
    }

    void
    mateWith( Individual & other )
    {
        assert( size() == other.size() );
       
        int middle = size() / 2;

        Individual tmp( *this );

        for (unsigned i = middle; i < size(); i++)
        {
            chromosomes_[i] = other.chromosomes_[i];
            other.chromosomes_[i] = tmp.chromosomes_[i];
        }
    }

    void
    mutate()
    {
        int randomIndex = rand() % chromosomes_.size();
        chromosomes_[randomIndex].shuffle();
    }

    float 
    evaluate( std::vector< Evaluator * > evaluators, bool verbose = false ) const
    {
        float overallFitness = 0;

        for ( unsigned i = 0; i < evaluators.size(); i++ )
        {
			float fitness = evaluators[i]->evaluate(*this);

			if (verbose)
			{
				std::cout << evaluators[i]->getName() << ":\t" << fitness << std::endl;
			}

			overallFitness += fitness;
        }
        return overallFitness / (float) evaluators.size();
    }

	std::vector< Chromosome >
	slice(unsigned index, unsigned sliceLength)
	{
		return std::vector< Chromosome > (
			chromosomes_.begin() +  index      * sliceLength,
			chromosomes_.begin() + (index + 1) * sliceLength);
	}

	unsigned
	numberOfSlices(unsigned sliceLength) const
	{
		return size() / sliceLength;
	}

	Chromosome
	operator()( unsigned slice, unsigned note, unsigned sliceLength ) const
	{
		return chromosomes_[slice * sliceLength + note];
	}

	unsigned
	count(Note note) const
	{
		unsigned cnt = 0;
		for (unsigned i = 0; i < size(); i++)
		{
			if (operator[](i).note() == note)
			{
				cnt++;
			}
		}
		return cnt;
	}

	unsigned
	count(Note note, unsigned slice, unsigned sliceLength) const
	{
		unsigned cnt = 0;
		for (unsigned i = 0; i < sliceLength; i++)
		{
			if (operator()(slice, i, sliceLength).note() == note)
			{
				cnt++;
			}
		}
		return cnt;
	}

    void
    print()
    {
        for ( Chromosome chromosome : chromosomes_ )
        {
            chromosome.print();
        }
        std::cout << std::endl;
    }

	void
	print(unsigned sliceLength)
	{
		for (unsigned slice = 0; slice < numberOfSlices(sliceLength); slice++)
		{
			for (unsigned note = 0; note < sliceLength; note++)
			{
				operator()(slice, note, sliceLength).print();
			}
			std::cout << std::endl;
		}
	}

private:
    std::vector< Chromosome > chromosomes_;
};


class Population
{
public:

    Population() {}
        
    Population( int populationSize, int individualSize )
    {
        for ( int i = 0; i < populationSize; i++ )
        {
            Individual individual( individualSize );
            individuals_.push_back( individual );
        }
    }

    void
    add( Individual individual )
    {
        individuals_.push_back( individual );
    }

    int 
    size()
    {
        return individuals_.size();
    }

    void
    mutate ( float mutationProbability )
    {
        assert( mutationProbability >= 0.0 && mutationProbability <= 1.0 );

        for ( Individual & individual : individuals_ )
        {
           float mutationRandomness = static_cast< float > ( rand() / static_cast< float > ( RAND_MAX ) );
            if ( mutationRandomness <= mutationProbability )
            {
                individual.mutate();
            }
        }
    }

    Individual
    select ( int tournamentSize, std::vector< Evaluator * > evaluators )
    {
        int randomPopulationIdx = rand() % size();
        Individual best = individuals_[randomPopulationIdx];
        float bestFitness = best.evaluate( evaluators );

        for ( int j = 1; j < tournamentSize; j++ )
        {
            randomPopulationIdx = rand() % size();
            Individual challenger = individuals_[randomPopulationIdx];

            float challengerFitness = challenger.evaluate( evaluators );
            if ( challengerFitness > bestFitness )
            {
                bestFitness = challengerFitness;
                best = challenger;
            }
        }

        return best;
    }

    Individual
    getBestIndividual( std::vector< Evaluator * > evaluators )
    {
        int bestIndividualIdx = 0;
        float bestFitness = 0;

        for ( int i = 0; i < size(); i++ )
        {
            float challengerFitness = individuals_[i].evaluate( evaluators );
            if ( challengerFitness > bestFitness )
            {
                bestIndividualIdx = i;
                bestFitness = challengerFitness;
            }
        }
        return individuals_[bestIndividualIdx];

    }

    void
    print()
    {
        for ( Individual individual : individuals_ )
        {
            individual.print();
        }
    }

private:

    std::vector< Individual > individuals_;
};

} // namespace ambience

#endif
