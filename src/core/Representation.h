#ifndef REPRESENTATION_H
#define REPRESENTATION_H

#include <array>
#include <vector>
#include <map>
#include <iostream>
#include <cassert>

namespace ambience
{

enum class Note
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
    B,
    REST,
    HOLD,
    NUMBER_OF_NOTES
};


const std::map< Note, std::string > 
noteToString = {
    { Note::C,    "C" },
    { Note::Db,   "Db" },
    { Note::D,    "D" },
    { Note::Eb,   "Eb" },
    { Note::E,    "E" },
    { Note::F,    "F" },
    { Note::Gb,   "Gb" },
    { Note::G,    "G" },
    { Note::Ab,   "Ab" },
    { Note::A,    "A" },
    { Note::B,    "B" },
    { Note::REST, "REST" },
    { Note::HOLD, "HOLD" }
};


class Individual;
class Chromosome;
class Population;


class Evaluator
{
public:
    virtual ~Evaluator(){}
    virtual float evaluate( const Individual & individual ) = 0;
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
    
    // TODO: Rule of three?
    Individual()
    {}

    Individual( const Individual & individual ) :
        chromosomes_( individual.chromosomes_ )
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

    int
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

        for (int i = middle; i < size(); i++)
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
    evaluate( std::vector< Evaluator * > evaluators )
    {
        float fitness = 0;
        for ( unsigned i = 0; i < evaluators.size(); i++ )
        {
            fitness += evaluators[i]->evaluate( *this );
        }
        return fitness;
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

    Population( const Population & other ) :
        individuals_( other.individuals_ )
    {}
        
    Population( int populationSize, int individualSize )
    {
        for ( int i = 0; i < populationSize; i++ )
        {
            Individual individual( individualSize );
            individuals_.push_back( individual );
        }
    }

    Population & operator= ( const Population & other )
    {
        if ( this != &other )
        {
            individuals_ = other.individuals_;
        }
        return *this;
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
