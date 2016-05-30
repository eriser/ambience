#include "Codec.h"
#include "Types.h"

int
main()
{
   writeWav(
         "/tmp/test.wav",
         NULL,
         16,
         2,
         44100 / 2,
         16
   );

   return 0;
}
