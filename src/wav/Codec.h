#ifndef CODEC_H
#define CODEC_H

#include "Types.h"

#include <string>
#include <cstdint>

void
writeWav (
      const std::string &  outputFilePath,
      real*                interleavedData,
      uint32_t             numSamples,
      uint16_t             numChannels,
      uint32_t             sampleRate,
      uint16_t             bitsPerSample
      );

#endif
