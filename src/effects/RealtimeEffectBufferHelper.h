#ifndef REALTIME_EFFECT_BUFFER_HELPER_H
#define REALTIME_EFFECT_BUFFER_HELPER_H

#include "../libraries/lib-utility/MemoryX.h"

class RealtimeEffectBufferHelper
{

    private:

    unsigned int chans;
    size_t numSamples;

    const static void deleteAllChannels(unsigned int chans, float** channeledBuf) {
        for (size_t i = 0; i < chans; i++) {
            delete[] channeledBuf[i];
        }

        delete[] channeledBuf;
    }

    public:

    // Allocate the in/out buffer arrays
    float** ibuf;
    float** obuf;
    float* temp;

    RealtimeEffectBufferHelper(float** buffers, const unsigned int chans, const size_t numSamples) {

        this->chans = chans;
        this->numSamples = numSamples;

        // Allocate the in/out buffer arrays
        ibuf = safenew float* [chans];
        obuf = safenew float* [chans];
        temp = safenew float[numSamples];

        const size_t memcpy_size = numSamples * sizeof(float);

        // Allocate new output buffers and copy buffer input into newly allocated input buffers
        for (unsigned int i = 0; i < chans; i++) {
            ibuf[i] = new float[numSamples];
            memcpy(ibuf[i], buffers[i], memcpy_size);
            obuf[i] = new float[numSamples];
        }
    }

    ~RealtimeEffectBufferHelper() {

        delete[] temp;

        deleteAllChannels(chans, obuf);

        deleteAllChannels(chans, ibuf);
    }
};

#endif
