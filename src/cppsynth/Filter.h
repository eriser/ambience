#ifndef FILTER_H
#define FILTER_H

#include "Types.h"

#define FILTER_BUFFER_LENGTH (4)

typedef enum
{
   LOWPASS = 0,
   BANDPASS,
   HIGHPASS,
   NUM_MODES,
} filter_mode_type;

typedef enum
{
   MINUS_12_DB = 0,
   MINUS_24_DB,
   NUM_STEEPNESSES
} filter_steepness_type;


class Filter
{
public:

   Filter();

   void reset();
   real getSample( real input );

   void setCutoff( real cutoff )
   {
      this->cutoff_ = cutoff;
      calculateFeedbackAmount();
   }

   real getCutoff()
   {
      return cutoff_;
   }

   void setCutoffMod( real cutoffMod )
   {
      cutoff_mod = cutoffMod;
      calculateFeedbackAmount();
   }

   real getCutoffMod()
   {
      return cutoff_mod;
   }

   void setResonance( real resonance )
   {
      this->resonance_ = resonance;
      calculateFeedbackAmount();
   }

   real getResonance()
   {
      return resonance_;
   }


private:

   real calculateCutoff();
   void calculateFeedbackAmount();

   real  a_buffer[FILTER_BUFFER_LENGTH];
   real  cutoff_;
   real  resonance_;
   real  cutoff_mod;
   real  feedback_amount;
   filter_mode_type     mode;
   filter_steepness_type   steepness;

};

#endif
