#ifndef CHROMOSOME_H
#define CHROMOSOME_H

#include "Representation.h"

namespace ambience
{

class Chromosome
{
public:

    Chromosome();
    Chromosome( Note note );

    Note note() const;
    void shuffle();
    void print();

private:

    Note note_;
};

}

#endif

