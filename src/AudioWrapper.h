#pragma once

#include "miniaudio/miniaudio.h"
#include "fftw/fftw3.h"

#include <cstdio>
#include <memory>

#include "AudioQueue.h"


namespace ChordFinder
{

class AudioWrapper
{
public:
    AudioWrapper();
    ~AudioWrapper();

    void test();

    std::shared_ptr<AudioQueue> getAudioQueue();

private:
    std::unique_ptr<ma_device> device;
    std::unique_ptr<ma_encoder> encoder;

    std::shared_ptr<AudioQueue> aqueue;

    /* Loopback mode is currently only supported on WASAPI. */
    ma_backend backends[1] = { ma_backend_wasapi };

};

}