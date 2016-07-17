#ifndef DELAY_H
#define DELAY_H

#include "Types.h"
#include <cstring>

class Delay
{
public:
    
    Delay( float amount, float timeInMS, 
           float feedback, bool adaptive, 
           float maxTimeInMS, unsigned samplerate ) :
        amount_( amount ), timeInMS_( timeInMS ), 
        feedback_( feedback ), adaptive_( adaptive ),
        samplerate_( samplerate )
    {
        bufferLength_ = (unsigned) ((maxTimeInMS / 1000.0f) * (float) samplerate_);
        buffer_ = new real[ bufferLength_ ];
        bufferPosition_ = 0;
        memset( buffer_, 0, bufferLength_ * sizeof( real ) );
    }

    ~Delay()
    {
        delete [] buffer_;
    }

    real getSample( real inputSample )
    {
        unsigned timeInSamples = (unsigned) ((timeInMS_ / 1000.0f) * (float) samplerate_);
        unsigned delaySamplePosition = bufferPosition_ % timeInSamples;

        real outputSample;        

        if ( adaptive_ )
        {
            outputSample = inputSample + (real) amount_ * buffer_[ delaySamplePosition ];
        }
        else
        {
            outputSample = (real) (1.0f - amount_) * inputSample + (real) amount_ * buffer_[ delaySamplePosition ];
        }

        buffer_[ bufferPosition_ ] = (real) feedback_ * outputSample;

        bufferPosition_++;
        if ( bufferPosition_ >= timeInSamples )
        {
            bufferPosition_ = 0;
        }
    
        return outputSample;
    }

private:

    float    amount_;
    float    timeInMS_;
    float    feedback_;
    bool     adaptive_;
    unsigned samplerate_;
    real *   buffer_;
    unsigned bufferLength_;
    unsigned bufferPosition_;

};

#endif



