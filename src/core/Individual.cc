
#include "Chromosome.h"
#include "Individual.h"

#include <vector>
#include <string>


namespace ambience
{

Individual::Individual()
{}

Individual::Individual( int length )
{
    for ( int i = 0; i < length; i++ )
    {
        Chromosome chromosome;
        chromosomes_.push_back( chromosome );
    }
}

Chromosome &
Individual::operator[]( int i )
{
    return chromosomes_[i];
}

const
Chromosome &
Individual::operator[]( int i ) const
{
    return chromosomes_[i];
}

unsigned
Individual::size() const
{
    return chromosomes_.size();
}

void
Individual::mateWith( Individual & other )
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
Individual::mutate()
{
    int randomIndex = rand() % chromosomes_.size();
    chromosomes_[randomIndex].shuffle();
}

float 
Individual::getFitness() const
{
    return fitness_;
}

void
Individual::evaluate(std::vector< Evaluator * > & evaluators, bool verbose )
{

    float overallFitness = 0;

    for (unsigned i = 0; i < evaluators.size(); i++)
    {
        float fitness = evaluators[i]->evaluate(*this);

        if (verbose)
        {
            std::cout << evaluators[i]->getName() << ":\t" << fitness << std::endl;
        }

        overallFitness += fitness;
    }
    fitness_ = overallFitness / (float)evaluators.size();
}

std::vector< Chromosome >
Individual::slice(unsigned index, unsigned sliceLength)
{
    return std::vector< Chromosome > (
        chromosomes_.begin() +  index      * sliceLength,
        chromosomes_.begin() + (index + 1) * sliceLength);
}

unsigned
Individual::numberOfSlices(unsigned sliceLength) const
{
    return size() / sliceLength;
}

Chromosome
Individual::operator()( unsigned slice, unsigned note, unsigned sliceLength ) const
{
    return chromosomes_[slice * sliceLength + note];
}

unsigned
Individual::count(Note note) const
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
Individual::count(Note note, unsigned slice, unsigned sliceLength) const
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
Individual::print()
{
    for ( Chromosome & chromosome : chromosomes_ )
    {
        chromosome.print();
    }
    std::cout << std::endl;
}

void
Individual::print(unsigned sliceLength)
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

}
