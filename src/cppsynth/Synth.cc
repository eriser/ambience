#include "Synth.h"

#include <iostream>
#include <string>

Synth::Synth(unsigned numVoices, unsigned oscillatorsPerVoice, int samplerate) :
	lfo(samplerate)
{
	this->samplerate = samplerate;
	this->oscillatorsPerVoice = oscillatorsPerVoice;

	for (unsigned i = 0; i < numVoices; i++)
	{
		a_voices.push_back(new Voice(oscillatorsPerVoice, samplerate));
	}

	lfo_to_volume = 0.0;
	lfo_to_filter = 0.0;
}

void Synth::reset()
{
	for (unsigned i = 0; i < a_voices.size(); i++)
	{
		a_voices[i]->reset();
	}
	lfo.reset();
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

void Synth::printParameters()
{
	printParameters("untitled");
}

void Synth::printParameters(const std::string & name)
{
	std::cout << 
		"Synth Parameters " << name <<
	    "\n----------------"
	<< std::endl;

	for (unsigned i = 0; i < oscillatorsPerVoice; i++)
	{
		std::cout <<
			"\n  Oscillator " << i << ":"
			"\n    - Misc:"
			"\n      + Waveform: " << waveFormToString(getWaveform(i)) <<
			"\n      + Detune:   " << getDetune(i) <<
			"\n      + Attack:   " << getAttack(i) <<
			"\n      + Decay:    " << getDecay(i) <<
			"\n      + Sustain:  " << getSustain(i) <<
			"\n      + Release:  " << getRelease(i) <<
			"\n    - Filter:"
			"\n      + Cutoff:   " << getCutoff(i) <<
			"\n      + Envelope: " << getFilterEnvelopeAmount(i) <<
			"\n      + Attack:   " << getFilterAttack(i) <<
			"\n      + Decay:    " << getFilterDecay(i) <<
			"\n      + Sustain:  " << getFilterSustain(i) <<
			"\n      + Release:  " << getFilterRelease(i) <<
		std::endl;
	}
}

/////////////
// SETTERS //
/////////////

void Synth::setCutoff(real frequency)
{
	for (unsigned i = 0; i < oscillatorsPerVoice; i++)
	{
		setCutoff(frequency, i);
	}
}

void Synth::setCutoff(real frequency, int oscillatorId)
{
	for (unsigned i = 0; i < a_voices.size(); i++)
	{
		a_voices[i]->a_filters[oscillatorId]->setCutoff(frequency / (samplerate / 2.0));
	}
}

void Synth::setWaveform(waveform_type waveform)
{
	for (unsigned i = 0; i < oscillatorsPerVoice; i++)
	{
		setWaveform(waveform, i);
	}
}

void Synth::setWaveform(waveform_type waveform, int oscillatorId)
{
	for (unsigned i = 0; i < a_voices.size(); i++)
	{
		a_voices[i]->a_oscillators[oscillatorId]->setWaveform(waveform);
	}
}

void Synth::setDetune(real cents, int oscillatorId)
{
	for (unsigned i = 0; i < a_voices.size(); i++)
	{
		a_voices[i]->a_oscillators[oscillatorId]->setDetune(cents);
	}
}

void Synth::setAttack(real attack)
{
	for (unsigned i = 0; i < oscillatorsPerVoice; i++)
	{
		setAttack(attack, i);
	}
}

void Synth::setAttack(real attack, int oscillatorId)
{
	for (unsigned i = 0; i < a_voices.size(); i++)
	{
		a_voices[i]->a_volume_envelopes[oscillatorId]->setAttack(attack);
	}
}

void Synth::setDecay(real decay)
{
	for (unsigned i = 0; i < oscillatorsPerVoice; i++)
	{
		setDecay(decay, i);
	}
}

void Synth::setDecay(real decay, int oscillatorId)
{
	for (unsigned i = 0; i < a_voices.size(); i++)
	{
		a_voices[i]->a_volume_envelopes[oscillatorId]->setDecay(decay);
	}
}

void Synth::setSustain(real sustain)
{
	for (unsigned i = 0; i < oscillatorsPerVoice; i++)
	{
		setSustain(sustain, i);
	}
}

void Synth::setSustain(real sustain, int oscillatorId)
{
	for (unsigned i = 0; i < a_voices.size(); i++)
	{
		a_voices[i]->a_volume_envelopes[oscillatorId]->setSustain(sustain);
	}
}

void Synth::setRelease(real release)
{
	for (unsigned i = 0; i < oscillatorsPerVoice; i++)
	{
		setRelease(release, i);
	}
}

void Synth::setRelease(real release, int oscillatorId)
{
	for (unsigned i = 0; i < a_voices.size(); i++)
	{
		a_voices[i]->a_volume_envelopes[oscillatorId]->setRelease(release);
	}
}

void Synth::setFilterAttack(real attack)
{
	for (unsigned i = 0; i < oscillatorsPerVoice; i++)
	{
		setFilterAttack(attack, i);
	}
}

void Synth::setFilterAttack(real attack, int oscillatorId)
{
	for (unsigned i = 0; i < a_voices.size(); i++)
	{
		a_voices[i]->a_filter_envelopes[oscillatorId]->setAttack(attack);
	}
}

void Synth::setFilterDecay(real decay)
{
	for (unsigned i = 0; i < oscillatorsPerVoice; i++)
	{
		setFilterDecay(decay, i);
	}
}

void Synth::setFilterDecay(real decay, int oscillatorId)
{
	for (unsigned i = 0; i < a_voices.size(); i++)
	{
		a_voices[i]->a_filter_envelopes[oscillatorId]->setDecay(decay);
	}
}

void Synth::setFilterSustain(real sustain)
{
	for (unsigned i = 0; i < oscillatorsPerVoice; i++)
	{
		setFilterSustain(sustain, i);
	}
}

void Synth::setFilterSustain(real sustain, int oscillatorId)
{
	for (unsigned i = 0; i < a_voices.size(); i++)
	{
		a_voices[i]->a_filter_envelopes[oscillatorId]->setSustain(sustain);
	}
}

void Synth::setFilterRelease(real release)
{
	for (unsigned i = 0; i < oscillatorsPerVoice; i++)
	{
		setFilterRelease(release, i);
	}
}

void Synth::setFilterRelease(real release, int oscillatorId)
{
	for (unsigned i = 0; i < a_voices.size(); i++)
	{
		a_voices[i]->a_filter_envelopes[oscillatorId]->setRelease(release);
	}
}

void Synth::setFilterEnvelopeAmount(real amount)
{
	for (unsigned i = 0; i < oscillatorsPerVoice; i++)
	{
		setFilterEnvelopeAmount(amount, i);
	}
}

void Synth::setFilterEnvelopeAmount(real amount, int oscillatorId)
{
	for (unsigned i = 0; i < a_voices.size(); i++)
	{
		a_voices[i]->a_filter_envelope_amounts[oscillatorId] = amount;
	}
}

/////////////
// GETTERS //
/////////////

real Synth::getCutoff(int oscillatorId)
{
	return a_voices[0]->a_filters[oscillatorId]->getCutoff() * (samplerate / 2.0);
}

waveform_type Synth::getWaveform(int oscillatorId)
{
	return a_voices[0]->a_oscillators[oscillatorId]->getWaveform();
}

real Synth::getDetune(int oscillatorId)
{
	return a_voices[0]->a_oscillators[oscillatorId]->getDetune();
}

real Synth::getAttack(int oscillatorId)
{
	return a_voices[0]->a_volume_envelopes[oscillatorId]->getAttack();
}

real Synth::getDecay(int oscillatorId)
{
	return a_voices[0]->a_volume_envelopes[oscillatorId]->getDecay();
}

real Synth::getSustain(int oscillatorId)
{
	return a_voices[0]->a_volume_envelopes[oscillatorId]->getSustain();
}

real Synth::getRelease(int oscillatorId)
{
	return a_voices[0]->a_volume_envelopes[oscillatorId]->getRelease();
}

real Synth::getFilterEnvelopeAmount(int oscillatorId)
{
	return a_voices[0]->a_filter_envelope_amounts[oscillatorId];
}

real Synth::getFilterAttack(int oscillatorId)
{
	return a_voices[0]->a_filter_envelopes[oscillatorId]->getAttack();
}

real Synth::getFilterDecay(int oscillatorId)
{
	return a_voices[0]->a_filter_envelopes[oscillatorId]->getDecay();
}

real Synth::getFilterSustain(int oscillatorId)
{
	return a_voices[0]->a_filter_envelopes[oscillatorId]->getSustain();
}

real Synth::getFilterRelease(int oscillatorId)
{
	return a_voices[0]->a_filter_envelopes[oscillatorId]->getRelease();
}