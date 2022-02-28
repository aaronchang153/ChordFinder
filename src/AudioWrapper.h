#pragma once

#include "miniaudio/miniaudio.h"
#include "fftw/fftw3.h"

#include <cstdio>
#include <memory>

#include "AudioQueue.h"
#include "PCMAnalyzer.h"


namespace ChordFinder
{

class AudioWrapper
{
public:
    AudioWrapper();
    ~AudioWrapper();

    void start();
    void test();

    std::shared_ptr<AudioQueue> getAudioQueue();
    std::shared_ptr<PCMAnalyzer> getPCMAnalyzer();

    //For filtering since I only care about the range A0-C8
    //static const float FREQ_LO = 27.5f;
    //static const float FREQ_HI = 4186.0f;

private:
    std::unique_ptr<ma_device> device;
    std::unique_ptr<ma_encoder> encoder;

    std::shared_ptr<AudioQueue> aqueue;
    std::shared_ptr<PCMAnalyzer> analyzer;

    /* Loopback mode is currently only supported on WASAPI. */
    ma_backend backends[1] = { ma_backend_wasapi };

    const ma_uint32 channels = 1;
    const ma_uint32 sample_rate = 44100;

};

}