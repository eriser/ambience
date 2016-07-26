#include "Voice.h"

#include <cmath>

Voice::Voice(unsigned numberOfOscillators, int samplerate)
{
	for (unsigned i = 0; i < numberOfOscillators; i++)
	{
		Oscillator * osc = new Oscillator(samplerate);
		ADSR volumeADSR;
		Env * volumeEnvelope = new Env(volumeADSR, samplerate);
		Filter * filter = new Filter();
		ADSR filterADSR;
		Env * filterEnvelope = new Env(filterADSR, samplerate);
		a_oscillators.push_back(osc);
		a_volume_envelopes.push_back(volumeEnvelope);
		a_filters.push_back(filter);
		a_filter_envelopes.push_back(filterEnvelope);
		a_filter_envelope_amounts.push_back(0.0);
	}
	midi_note = -1;
	midi_velocity = 0;
}


void Voice::reset()
{
	for (unsigned i = 0; i < a_oscillators.size(); i++)
	{
		a_oscillators[i]->reset();
		a_volume_envelopes[i]->reset();
		a_filters[i]->reset();
		a_filter_envelopes[i]->reset();
		a_filter_envelope_amounts[i] = 0.0;
	}
}

bool Voice::isActive()
{
	for (unsigned i = 0; i < a_oscillators.size(); i++)
	{
		if (a_volume_envelopes[i]->getCurrentStage() != OFF)
		{
			return true;
		}

		if (a_filter_envelopes[i]->getCurrentStage() != OFF)
		{
			return true;
		}
	}
	return false;
}

bool Voice::isPressed()
{
	for (unsigned i = 0; i < a_oscillators.size(); i++)
	{
		if (a_volume_envelopes[i]->getCurrentStage() != OFF &&
			a_volume_envelopes[i]->getCurrentStage() != RELEASE)
		{
			return true;
		}

		if (a_filter_envelopes[i]->getCurrentStage() != OFF &&
			a_filter_envelopes[i]->getCurrentStage() != RELEASE)
		{
			return true;
		}
	}
	return false;
}

void Voice::setFilterLfoMod(real filterLfoMod)
{
	this->filter_lfo_mod = filterLfoMod;
}

void Voice::setMidiNote(int midiNote)
{
	this->midi_note = midiNote;
	real frequency = 440.0 * pow(2.0, (midi_note - 69.0) / 12.0);
	for (unsigned i = 0; i < a_oscillators.size(); i++)
	{
		a_oscillators[i]->setFrequency(frequency);
	}
}

int Voice::getMidiNote()
{
	return midi_note;
}

void Voice::noteOn()
{
	for (unsigned i = 0; i < a_oscillators.size(); i++)
	{
		a_volume_envelopes[i]->noteOn();
		a_filter_envelopes[i]->noteOn();
	}
}

void Voice::noteOff()
{
	for (unsigned i = 0; i < a_oscillators.size(); i++)
	{
		a_volume_envelopes[i]->noteOff();
		a_filter_envelopes[i]->noteOff();
	}
}

real Voice::getSample()
{
	if (!isActive())
	{
		return 0.0;
	}

	real sum_sample = 0.0;

	for (int i = 0; i < (int)a_oscillators.size(); i++)
	{
		real sample = a_oscillators[i]->getSample();

		real filter_mod = a_filter_envelopes[i]->getSample() * this->a_filter_envelope_amounts[i] + this->filter_lfo_mod;
		filter_mod = filter_mod >  1.0 ? 1.0 : filter_mod;
		filter_mod = filter_mod < -1.0 ? -1.0 : filter_mod;
		
		a_filters[i]->setCutoffMod(filter_mod);

		sample = a_filters[i]->getSample(sample);

		sample *= a_volume_envelopes[i]->getSample();

		sum_sample += sample;
	}

	sum_sample /= (real)a_oscillators.size();

	return sum_sample;
}
