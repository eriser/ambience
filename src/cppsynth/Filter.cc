
#include "Filter.h"
#include "Types.h"

#include <cstring>

Filter::Filter()
{
   memset(a_buffer, 0, sizeof(a_buffer));

   cutoff_ = 0.5;
   resonance_ = 0.0;
   cutoff_mod = 0.0;

   calculateFeedbackAmount();

   mode = LOWPASS;
   steepness = MINUS_24_DB;
}

void
Filter::reset()
{
   memset(a_buffer, 0, sizeof(a_buffer));
}


real
Filter::getSample( real input )
{
   if (input == 0.0)
   {
      return 0.0;
   }

   real cutoff = calculateCutoff();

   a_buffer[0] += cutoff * (input - a_buffer[0] + feedback_amount * (a_buffer[0] - a_buffer[1]));
   a_buffer[1] += cutoff * (a_buffer[0] - a_buffer[1]);

   if (steepness == MINUS_12_DB)
   {
      if (mode == LOWPASS)
      {
         return a_buffer[1];
      }
      else if (mode == BANDPASS)
      {
         return a_buffer[0] - a_buffer[1];
      }
      else if (mode == HIGHPASS)
      {
         return input - a_buffer[0];
      }

   }

   else if (steepness == MINUS_24_DB)
   {
      a_buffer[2] += cutoff * (a_buffer[1] - a_buffer[2]);
      a_buffer[3] += cutoff * (a_buffer[2] - a_buffer[3]);

      if (mode == LOWPASS)
      {
         return a_buffer[3];
      }
      else if (mode == BANDPASS)
      {
         return a_buffer[0] - a_buffer[3];
      }
      else if (mode == HIGHPASS)
      {
         return input - a_buffer[3];
      }
   }

   return input;
}


real
Filter::calculateCutoff()
{
   real val = cutoff_ + cutoff_mod;
   val = val < 0.0 ? 0.0 : val;
   val = val > 0.999 ? 0.999 : val;
   return val;
}


void
Filter::calculateFeedbackAmount()
{
   feedback_amount = resonance_ + resonance_ / ( 1.0 - calculateCutoff() );
}


