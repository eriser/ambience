#include "Osc.h"
#include "Types.h"

#include "Types.h"

#include <cmath>
#include <cstdlib>
#include <string>

std::string waveFormToString(waveform_type waveform)
{
	std::string waveformString;
	switch (waveform)
	{
	case SINE:
		waveformString = "Sine";
		break;
	case SAW:
		waveformString = "Saw";
		break;
	case SQUARE:
		waveformString = "Square";
		break;
	case TRIANGLE:
		waveformString = "Triangle";
		break;
	case WHITE_NOISE:
		waveformString = "White Noise";
		break;
	default:
		waveformString = "Invalid";
		break;
	}

	return waveformString;
}

Oscillator::Oscillator(int samplerate)
{
	waveform_ = SAW;
	phase = 0.0;
	this->samplerate = samplerate;
	last_output = 0.0;
	gain = 1.0;

	setFrequency(440.0);
	setDetune(0.0);

	update_increment();
}

void Oscillator::setFrequency(real frequency)
{
	this->dryFrequency_ = frequency;
    updateFrequency();
}

void Oscillator::setDetune(real detune)
{
	this->detune_ = detune;
    updateFrequency();
}

// combines detune and dryFrequency (frequency wihtout detune) to actual frequency
void Oscillator::updateFrequency()
{
    this->frequency_ =  dryFrequency_ * pow(2.0, (detune_ / 1200.0));
	update_increment();
}

real Oscillator::getSample()
{
	real two_pi = 2.0 * M_PI;
	real t = this->phase / two_pi;
	real value = 0.0;

	switch (this->waveform_)
	{
	case SINE:
		value = sin(this->phase);
		break;
	case SAW:
		value = (2.0 * t) - 1.0;
		value -= poly_blep(t);
		break;
	case SQUARE:
		if (this->phase < M_PI)
		{
			value = 1.0;
		}
		else
		{
			value = -1.0;
		}
		value += poly_blep(t);
		value -= poly_blep(fmod(t + 0.5, 1.0));
		break;
	case TRIANGLE:
		if (this->phase < M_PI)
		{
			value = 1.0;
		}
		else
		{
			value = -1.0;
		}
		value += poly_blep(t);
		value -= poly_blep(fmod(t + 0.5, 1.0));
		value = value * this->phase_increment + (1.0 - this->phase_increment) * this->last_output;
		break;
	case WHITE_NOISE:
		value = 2.0 *((real)rand()) / RAND_MAX - 1.0;
		break;
	}

	this->phase += this->phase_increment;
	while (this->phase > two_pi)
	{
		this->phase -= two_pi;
	}

	this->last_output = value;

	value *= this->gain;

	return value;
}

void Oscillator::reset()
{
	phase = 0.0;
	last_output = 0.0;
}

void Oscillator::update_increment()
{
	phase_increment = frequency_ * 2.0 * M_PI / (real)samplerate;
}

real Oscillator::poly_blep(real t)
{
	real dt = phase_increment / (2.0 * M_PI);

	if (t < dt)
	{
		t /= dt;
		return (2.0 - t) * t - 1.0;
	}
	else if (t > 1.0 - dt)
	{
		t = (t - 1.0) / dt;
		return (2.0 + t) * t + 1.0;
	}
	else
	{
		return 0.0;
	}
}
