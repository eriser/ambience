#ifndef EVOLUTION_H
#define EVOLUTION_H

#include <sstream>
#include <iomanip>
#include <cmath>

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
    run( int generations, float fitnessThreshold, bool verbose )
    {
        for ( int i = 0; i < generations; i++ )
        {
			population_.evaluate(evaluators_);

			float bestFitness = getBestIndividual().getFitness();

			if (verbose)
			{
				int stringWidth = (int)log10(generations) + 1;
				std::stringstream ss;
				ss << std::setw(stringWidth) << std::setfill(' ') << i + 1;
				std::string index = ss.str();

				ss.str("");
				ss.clear();
				ss << std::fixed << std::setprecision(3) << bestFitness;
				std::string fitnessString = ss.str();

				std::string output = "Iteration " + index + " / " + std::to_string(generations) + " (max fitness = " + fitnessString + ")";
				if (i > 0)
				{
					for (unsigned j = 0; j < output.size(); j++)
					{
						printf("\b");
					}
					std::cout.flush();
				}
				
				printf("%s", output.c_str());
				std::cout.flush();
			}

			if (bestFitness >= fitnessThreshold)
			{
				if (verbose)
				{
					std::cout << std::endl;
					std::cout << "Max fitness exceeded threshold after " << i << " iterations.";
				}
				break;
			}

            Population newPopulation;
            
            for ( int j = 0; j < population_.size() / 2; j++ )
            {
                // select parents from population
                int tournamentSize = 10;
                Individual parent1 = population_.select( tournamentSize );
                Individual parent2 = population_.select( tournamentSize );

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
			Individual best = getBestIndividual();
			best.evaluate( evaluators_, verbose=true );
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
        return population_.getBestIndividual();
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

