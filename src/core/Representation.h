#ifndef REPRESENTATION_H
#define REPRESENTATION_H

#include <array>
#include <vector>
#include <map>
#include <iostream>
#include <cassert>
#include <typeinfo>

namespace ambience
{

enum class Note
{
    ON,
    REST,
    HOLD,
    NUMBER_OF_NOTES
};

enum class NoteValue
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
	Bb,
	B,
	NUMBER_OF_NOTE_VALUES
};

const std::map< Note, std::string > 
noteToString = {
    { Note::ON,   "ON" },
    { Note::REST, "REST" },
    { Note::HOLD, "HOLD" }
};

extern
NoteValue
midiToNoteValue(unsigned midiNote);


} // namespace ambience

#endif
