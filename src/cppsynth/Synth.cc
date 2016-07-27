#include "Synth.h"

Synth::Synth(int numVoices, int oscillatorsPerVoice, int samplerate) :
	lfo(samplerate)
{
	this->samplerate = samplerate;

	for (int i = 0; i < numVoices; i++)
	{
		a_voices.push_back(new Voice(oscillatorsPerVoice, samplerate));
	}

	lfo_to_volume = 0.0;
	lfo_to_filter = 0.0;
}

void Synth::noteOn(int midiNote)
{
	int idx = getFreeVoiceIndex();

	if (idx < 0 || idx >= (int)a_voices.size())
	{
		return;
	}

	a_voices[idx]->reset();
	a_voices[idx]->setMidiNote(midiNote);
	a_voices[idx]->noteOn();
}

void Synth::noteOff(int midiNote)
{
	int idx = -1;

	for (int i = 0; i < (int)a_voices.size(); i++)
	{
		if (a_voices[i]->getMidiNote() == midiNote &&
			a_voices[i]->isPressed())
		{
			idx = i;
			break;
		}
	}

	if (idx < 0 || idx >= (int)a_voices.size())
	{
		return;
	}

	a_voices[idx]->noteOff();

}

real Synth::getSample()
{
	real sample = 0.0;

	real lfo_sample = lfo.getSample();

	for (unsigned i = 0; i < a_voices.size(); i++)
	{
		if (a_voices[i]->isActive())
		{
			real filter_lfo_mod = lfo_sample * lfo_to_filter;
			filter_lfo_mod = filter_lfo_mod >  1.0 ? 1.0 : filter_lfo_mod;
			filter_lfo_mod = filter_lfo_mod < -1.0 ? -1.0 : filter_lfo_mod;

			a_voices[i]->setFilterLfoMod(filter_lfo_mod);

			sample += a_voices[i]->getSample();
		}
	}

	sample /= a_voices.size();
	
	sample *= (lfo_sample * lfo_to_volume + 1.0) * 0.5;

	if (sample > 1.0)
	{
		sample = 1.0;
	}
	else if (sample < -1.0)
	{
		sample = -1.0;
	}

	return sample;
}

int Synth::getSampleRate()
{
	return samplerate;
}

int Synth::getFreeVoiceIndex()
{
	int free_voice = -1;

	for (int i = 0; i < (int)a_voices.size(); i++)
	{
		if (!a_voices[i]->isActive())
		{
			free_voice = i;
			break;
		}
	}

	return free_voice;
}

void Synth::setCutoff(real frequency)
{
    for (int i = 0; i < a_voices.size(); i++)
    {
        for (int j = 0; j < a_voices[i]->a_filters.size(); j++)
        {
            a_voices[i]->a_filters[j]->setCutoff( frequency / (samplerate / 2.0) );
        }
    }
}


void Synth::setDetune(real cents, int oscillatorId)
{
    for (int i = 0; i < a_voices.size(); i++)
    {
        a_voices[i]->a_oscillators[oscillatorId]->setDetune(cents);
    }
    
}
