
#include "Env.h"
#include <cmath>

Env::Env( ADSR & adsr, int samplerate ) :
      adsr_( adsr ), samplerate_( samplerate )
{
   adsr_.a_stage_value[OFF]        = 0.0;
   adsr_.a_stage_value[ATTACK]     = 0.1;
   adsr_.a_stage_value[DECAY]      = 0.5;
   adsr_.a_stage_value[SUSTAIN]    = 0.7;
   adsr_.a_stage_value[RELEASE]    = 2.0;

   adsr_.samplerate    = samplerate;

   adsr_.current_level = ENV_MIN_LEVEL;
   adsr_.min_level      = ENV_MIN_LEVEL;
   adsr_.current_stage = OFF;
   adsr_.multiplier    = 1.0;
   adsr_.current_sample_index      = 0;
   adsr_.next_stage_sample_index   = 0;
}


void
Env::reset()
{
   adsr_.current_stage = OFF;
   adsr_.current_level  = adsr_.min_level;
   adsr_.multiplier  = 1.0;
   adsr_.current_sample_index       = 0;
   adsr_.next_stage_sample_index    = 0;

}


void
Env::noteOn()
{
   enterStage( ATTACK );
}


void
Env::noteOff()
{
   enterStage( RELEASE );
}

stage_type
Env::getCurrentStage()
{
	return adsr_.current_stage;
}

real
Env::getSample()
{
   if (!(adsr_.current_stage == OFF ||
            adsr_.current_stage == SUSTAIN)
      )
   {
      if (adsr_.current_sample_index >= adsr_.next_stage_sample_index)
      {
         stage_type new_stage = static_cast<stage_type>((adsr_.current_stage + 1) % NUM_STAGES);
         enterStage( new_stage );
      }

      adsr_.current_level *= adsr_.multiplier;
      adsr_.current_sample_index += 1;
   }
   return adsr_.current_level;
}


void
Env::calculateMultiplier( real startLevel, real endLevel, unsigned long long lengthInSamples )
{
   adsr_.multiplier = 1.0 + (log(endLevel) - log(startLevel)) / (real) lengthInSamples;
}


void
Env::enterStage( stage_type new_stage )
{
   if (adsr_.current_stage == new_stage)
   {
      return;
   }

   adsr_.current_stage = new_stage;
   adsr_.current_sample_index = 0;

   if (adsr_.current_stage == OFF ||
         adsr_.current_stage == SUSTAIN)
   {
      adsr_.next_stage_sample_index = 0;
   }
   else
   {
      adsr_.next_stage_sample_index = (unsigned long long) (adsr_.a_stage_value[adsr_.current_stage] * (real)adsr_.samplerate);
   }

   if (new_stage == OFF)
   {
      adsr_.current_level = 0.0;
      adsr_.multiplier = 1.0;
   }

   else if (new_stage == ATTACK)
   {
      adsr_.current_level = adsr_.min_level;
      calculateMultiplier
         (adsr_.current_level,
          1.0,
          adsr_.next_stage_sample_index);
   }

   else if (new_stage == DECAY)
   {
      adsr_.current_level = 1.0;
      calculateMultiplier
         (adsr_.current_level,
          fmax(adsr_.a_stage_value[SUSTAIN], adsr_.min_level),
          adsr_.next_stage_sample_index);
   }

   else if (new_stage == SUSTAIN)
   {
      adsr_.current_level = adsr_.a_stage_value[SUSTAIN];
      adsr_.multiplier = 1.0;
   }
   else if (new_stage == RELEASE)
   {
      calculateMultiplier
         (adsr_.current_level,
          adsr_.min_level,
          adsr_.next_stage_sample_index);
   }
}
