
#include "Representation.h"
#include "Chromosome.h"
#include "Individual.h"
#include "Population.h"

#include <vector>

namespace ambience
{

Population::Population() {}
    
Population::Population( int populationSize, unsigned numberOfSlices, unsigned sliceLength ) :
	numberOfSlices_( numberOfSlices ), sliceLength_( sliceLength )
{
    for ( int i = 0; i < populationSize; i++ )
    {
        Individual individual( numberOfSlices, sliceLength );
        individuals_.push_back( individual );
    }
}

void
Population::add( Individual & individual )
{
    individuals_.push_back( individual );
}

int 
Population::size()
{
    return individuals_.size();
}

void
Population::mutate ( float mutationProbability )
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
Population::select ( int tournamentSize )
{
    int randomPopulationIdx = rand() % size();
    Individual best = individuals_[randomPopulationIdx];
    float bestFitness = best.getFitness();

    for ( int j = 1; j < tournamentSize; j++ )
    {
        randomPopulationIdx = rand() % size();
        Individual challenger = individuals_[randomPopulationIdx];

        float challengerFitness = challenger.getFitness();
        if ( challengerFitness > bestFitness )
        {
            bestFitness = challengerFitness;
            best = challenger;
        }
    }

    return best;
}

Individual
Population::getBestIndividual()
{
    int bestIndividualIdx = 0;
    float bestFitness = 0;

    for ( int i = 0; i < size(); i++ )
    {
        float challengerFitness = individuals_[i].getFitness();
        if ( challengerFitness > bestFitness )
        {
            bestIndividualIdx = i;
            bestFitness = challengerFitness;
        }
    }
    return individuals_[bestIndividualIdx];

}

void
Population::print()
{
    for ( Individual & individual : individuals_ )
    {
        individual.print();
    }
}

void
Population::evaluate(std::vector< Evaluator * > & evaluators, bool verbose )
{
    for (unsigned i = 0; i < individuals_.size(); i++)
    {
        individuals_[i].evaluate(evaluators, verbose);
    }
}

}
