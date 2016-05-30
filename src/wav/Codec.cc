
#include "Types.h"

#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <cmath>

inline
static
uint32_t
swap_uint32 (
      uint32_t val
      )
{
    val = ((val << 8) & 0xFF00FF00 ) | ((val >> 8) & 0xFF00FF );
    return (val << 16) | (val >> 16);
}

inline
static
uint16_t
swap_uint16 (
      uint16_t val
      )
{
    return (val << 8) | (val >> 8 );
}

inline
static
void
streamWrite (
      std::ofstream &   outputFile,
      int16_t          data
      )
{
   outputFile.write( (const char *) &data, sizeof( data ) );
}

inline
static
void
streamWrite (
      std::ofstream &   outputFile,
      uint32_t          data
      )
{
   outputFile.write( (const char *) &data, sizeof( data ) );
}

inline
static
void
streamWrite (
      std::ofstream &   outputFile,
      uint16_t          data
      )
{
   outputFile.write( (const char *) &data, sizeof( data ) );
}

void
writeWav (
      const std::string &  outputFilePath,
      real*                interleavedData,
      uint32_t             numSamples,       // for stereo, 1 sample == 2 entries in the interleaved data
      uint16_t             numChannels,
      uint32_t             sampleRate,
      uint16_t             bitsPerSample
      )
{

   // RIFF header
   uint32_t riff           = swap_uint32( 0x52494646 );  // "RIFF"
   uint32_t fileSize;                                    // filled afterwards
   uint32_t wave           = swap_uint32( 0x57415645 );  // "WAVE"

   // fmt chunk
   uint32_t fmt            = swap_uint32( 0x666d7420 );  // "fmt "
   uint32_t fmtChunkSize   = 16;                         // size of the rest of the fmt chunk in bytes
   uint16_t audioFormat    = 1;                          // 1 for PCM
   // numChannels
   // sampleRate
   uint32_t byteRate       = sampleRate * numChannels * bitsPerSample / 8;
   uint16_t blockAlign     = numChannels * bitsPerSample / 8;
   // bitsPerSample

   // data chunk
   uint32_t data           = swap_uint32( 0x64617461 );  // "data"
   uint32_t dataSize       = numSamples * numChannels * bitsPerSample / 8;


   // calculate file size
   fileSize = 36 + dataSize;

   // writing to file
   std::ofstream outputFile( outputFilePath.c_str(), std::ios::out | std::ios::binary );

   if ( outputFile.is_open() )
   {

      streamWrite( outputFile, riff );
      streamWrite( outputFile, fileSize );
      streamWrite( outputFile, wave );

      streamWrite( outputFile, fmt );
      streamWrite( outputFile, fmtChunkSize );
      streamWrite( outputFile, audioFormat );
      streamWrite( outputFile, numChannels );
      streamWrite( outputFile, sampleRate );
      streamWrite( outputFile, byteRate );
      streamWrite( outputFile, blockAlign );
      streamWrite( outputFile, bitsPerSample );

      streamWrite( outputFile, data );
      streamWrite( outputFile, dataSize );

      real multiplier = pow( 2.0, (real) bitsPerSample );
      for ( unsigned sample = 0; sample < numSamples; sample += numChannels )
      {
         for ( unsigned channel = 0; channel < numChannels; channel++ )
         {
            int16_t audiodata = (int16_t) (interleavedData[ sample + channel ] * multiplier);
            streamWrite( outputFile, (int16_t) audiodata );
         }
      }

      outputFile.close();
   }

}

