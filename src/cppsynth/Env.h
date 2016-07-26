#ifndef ENV_H
#define ENV_H

#include "Types.h"

#define ENV_MIN_LEVEL (0.0001)

typedef enum
{
   OFF = 0,
   ATTACK,
   DECAY,
   SUSTAIN,
   RELEASE,
   NUM_STAGES
} stage_type;

class ADSR
{

   friend class Env;

private:
   real                 a_stage_value[NUM_STAGES];
   real                 min_level;
   stage_type           current_stage;
   real                 current_level;
   real                 multiplier;
   int                  samplerate;
   unsigned long long   current_sample_index;
   unsigned long long   next_stage_sample_index;
};

class Env
{
public:
   Env( ADSR & adsr, int samplerate );

   void reset();

   void noteOn();
   void noteOff();

   real getSample();
   stage_type getCurrentStage();

private:

   void calculateMultiplier( real startLevel, real endLevel, unsigned long long lengthInSamples );
   void enterStage( stage_type newStage );

   ADSR adsr_;

};

#endif
