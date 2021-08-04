#ifndef REALTIME_EFFECT_BUFFER_HELPER_H
#define REALTIME_EFFECT_BUFFER_HELPER_H

#include "../libraries/lib-utility/MemoryX.h"
#include <string>

class RealtimeEffectBufferHelper
{

    private:

    unsigned int chans;
    size_t numSamples;

    const static void deleteAllChannels(unsigned int chans, float** channeledBuf) {
        delete[] channeledBuf[0];
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

        const size_t memcpy_size = (size_t)numSamples * chans * sizeof(float);

        ibuf[0] = safenew float[(size_t)chans * numSamples];
        obuf[0] = safenew float[(size_t)chans * numSamples];

        memset(ibuf[0], 0, memcpy_size);
        memset(obuf[0], 0, memcpy_size);

        // Allocate new output buffers and copy buffer input into newly allocated input buffers

        for (unsigned int i = 1; i < chans; i++) {
            ibuf[i] = ibuf[i - 1] + numSamples;
            obuf[i] = obuf[i - 1] + numSamples;
        }

        for (size_t j = 0; j < chans; j++) {
            memcpy(ibuf[j], buffers[j], (size_t)numSamples * sizeof(float));
        }
    }

    ~RealtimeEffectBufferHelper() {

        delete[] temp;

        deleteAllChannels(chans, obuf);

        deleteAllChannels(chans, ibuf);
    }
};

#endif
