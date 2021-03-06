#ifndef CODEC_H
#define CODEC_H

#include "Types.h"

#include <string>
#include <cstdint>
#include <vector>

void
writeWav (
      const std::string &  outputFilePath,
      std::vector<real> &  interleavedData,
      uint16_t             numChannels,
      uint32_t             sampleRate,
      uint16_t             bitsPerSample
      );

#endif
