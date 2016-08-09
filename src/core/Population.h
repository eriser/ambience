#ifndef POPULATION_H
#define POPULATION_H

#include "Representation.h"
#include "Chromosome.h"
#include "Individual.h"

#include <vector>

namespace ambience
{

class Population
{
public:

    Population();
        
    Population( int populationSize, unsigned numberOfSlices, unsigned sliceLength );

    void
    add( Individual & individual );

    int 
    size();

    void
    mutate ( float mutationProbability );

    Individual
    select ( int tournamentSize );

    Individual
    getBestIndividual();

    void
    print();

	void
	evaluate(std::vector< Evaluator * > & evaluators, bool verbose = false);

private:

	unsigned numberOfSlices_;
	unsigned sliceLength_;
    std::vector< Individual > individuals_;

};

}

#endif
