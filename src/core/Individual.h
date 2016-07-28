#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include "Chromosome.h"
#include "Evaluator.h"

#include <vector>

namespace ambience
{

class Individual
{
public:

    Individual();

    Individual( int length );

    Chromosome &
    operator[]( int i );

    const
    Chromosome &
    operator[]( int i ) const;

    unsigned
    size() const;

    void
    mateWith( Individual & other );

    void
    mutate();

    float 
    getFitness() const;

	void
	evaluate(std::vector< Evaluator * > & evaluators, bool verbose = false);

	std::vector< Chromosome >
	slice(unsigned index, unsigned sliceLength);

	unsigned
	numberOfSlices(unsigned sliceLength) const;

	Chromosome
	operator()( unsigned slice, unsigned note, unsigned sliceLength ) const;

	unsigned
	count(Note note) const;

	unsigned
	count(Note note, unsigned slice, unsigned sliceLength) const;

    void
    print();

	void
	print(unsigned sliceLength);

private:
    std::vector< Chromosome > chromosomes_;
	float fitness_;
};

}


#endif
