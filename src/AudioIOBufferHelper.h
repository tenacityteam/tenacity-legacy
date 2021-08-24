#ifndef AUDIOIO_BUFFER_HELPER_H
#define AUDIOIO_BUFFER_HELPER_H

#include "AudioIO.h"
#include "../libraries/lib-utility/MemoryX.h"

class AudioIOBufferHelper
{

    private:

        unsigned int numPlaybackChannels;
        unsigned long framesPerBuffer;

    public:
        WaveTrack** chans;
        float** tempBufs;

        AudioIOBufferHelper(const unsigned int numPlaybackChannels, const unsigned long framesPerBuffer) {
            this->numPlaybackChannels = numPlaybackChannels;
            this->framesPerBuffer = framesPerBuffer;

            this->chans = safenew WaveTrack * [numPlaybackChannels];
            this->tempBufs = safenew float* [numPlaybackChannels];

            for (unsigned int c = 0; c < numPlaybackChannels; c++) {
                tempBufs[c] = safenew float[framesPerBuffer];
            }
        }

        ~AudioIOBufferHelper() {
            for (unsigned int c = 0; c < numPlaybackChannels; c++) {
                delete[] tempBufs[c];
            }

            delete[] tempBufs;

            delete[] chans;
        }
};

#endif
