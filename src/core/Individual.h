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

    Individual( unsigned numberOfSlices, unsigned sliceLength );

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
	slice(unsigned index);

	unsigned
	numberOfSlices() const;

	unsigned
	sliceLength() const { return sliceLength_; }

	Chromosome
	operator()( unsigned slice, unsigned note) const;

	unsigned
	count(Note note) const;

	unsigned
	count(Note note, unsigned slice) const;

    void
    print();

private:
	unsigned numberOfSlices_;
	unsigned sliceLength_;
    std::vector< Chromosome > chromosomes_;
	float fitness_;
};

}


#endif
