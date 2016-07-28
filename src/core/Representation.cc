
#include "Representation.h"

namespace ambience
{

NoteValue
midiToNoteValue(unsigned midiNote)
{
	return static_cast<NoteValue>(midiNote % static_cast<unsigned>(NoteValue::NUMBER_OF_NOTE_VALUES));
}

}

