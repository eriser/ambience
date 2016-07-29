#include "Chromosome.h"
#include "Representation.h"

#include <array>
#include <vector>
#include <map>
#include <iostream>
#include <cassert>
#include <typeinfo>
#include <string>

using namespace ambience;


Chromosome::Chromosome()
{
    shuffle();
}

Chromosome::Chromosome( Note note ) :
    note_( note )
{}

Note
Chromosome::note() const
{
    return note_;
}

void 
Chromosome::shuffle()
{
    int numberOfNotes = static_cast< int >( Note::NUMBER_OF_NOTES );
    note_ = static_cast< ambience::Note >( rand() % numberOfNotes ); 
}

void
Chromosome::print()
{
    std::cout << noteToString.at(note_) << " ";
}
