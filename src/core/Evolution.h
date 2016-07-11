#ifndef EVOLUTION_H
#define EVOLUTION_H

#include <sstream>
#include <iomanip>

#include "Representation.h"

namespace ambience
{

class GeneticAlgorithmRunner
{
public:

    GeneticAlgorithmRunner( int populationSize, int individualSize ) :
        population_( populationSize, individualSize )
    {}
    
    void
    run( int generations, bool verbose )
    {
        for ( int i = 0; i < generations; i++ )
        {
			if (verbose)
			{
				int stringWidth = (int)log10(generations) + 1;
				std::stringstream ss;
				ss << std::setw(stringWidth) << std::setfill(' ') << i + 1;
				std::string index = ss.str();

				std::string output = "Iteration " + index + " / " + std::to_string(generations);
				if (i > 0)
				{
					for (int i = 0; i < output.size(); i++)
					{
						printf("\b");
					}
					std::cout.flush();
				}
				
				printf("%s", output);
				std::cout.flush();
			}

            Population newPopulation;
            
            for ( int i = 0; i < population_.size() / 2; i++ )
            {
                // select parents from population
                int tournamentSize = 6;
                Individual parent1 = population_.select( tournamentSize, evaluators_ );
                Individual parent2 = population_.select( tournamentSize, evaluators_ );

                // mate and insert into population
                parent1.mateWith( parent2 );
                newPopulation.add( parent1 );
                newPopulation.add( parent2 );
            }
                
            newPopulation.mutate( 1.0 );
            population_ = newPopulation;
        }

		if (verbose)
		{
			std::cout << std::endl;
		}
    }

    void
    registerEvaluator( ambience::Evaluator & evaluator )
    {
        evaluators_.push_back( &evaluator );
    }

    Individual
    getBestIndividual()
    {
        return population_.getBestIndividual( evaluators_ );
    }

    float
    evaluateIndividual( Individual & individual )
    {
        return individual.evaluate( evaluators_ ); 
    }

    void
    printPopulation()
    {
        population_.print();
    }

private:
    
    ambience::Population population_;
    std::vector< ambience::Evaluator * > evaluators_;

};

} // namespace ambience

#endif

