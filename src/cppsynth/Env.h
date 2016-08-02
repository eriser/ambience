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

   void setAttack(real attack)   { adsr_.a_stage_value[ATTACK]  = attack; }
   void setDecay(real decay)     { adsr_.a_stage_value[DECAY]   = decay; }
   void setSustain(real sustain) { if (sustain > 1.0) sustain = 1.0; if (sustain < 0.0) sustain = 0.0; adsr_.a_stage_value[SUSTAIN] = sustain; }
   void setRelease(real release) { adsr_.a_stage_value[RELEASE] = release; }

   real getAttack()  { return adsr_.a_stage_value[ATTACK]; }
   real getDecay()   { return adsr_.a_stage_value[DECAY]; }
   real getSustain() { return adsr_.a_stage_value[SUSTAIN]; }
   real getRelease() { return adsr_.a_stage_value[RELEASE]; }

private:

   void calculateMultiplier( real startLevel, real endLevel, unsigned long long lengthInSamples );
   void enterStage( stage_type newStage );

   ADSR adsr_;

};

#endif
