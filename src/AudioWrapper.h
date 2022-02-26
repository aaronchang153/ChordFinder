#pragma once

#include "miniaudio/miniaudio.h"
#include "fftw/fftw3.h"

#include <cstdio>
#include <memory>


namespace ChordFinder
{

class AudioWrapper
{
public:
    AudioWrapper();
    ~AudioWrapper();

    void test();

private:
    std::unique_ptr<ma_device> device;
    std::unique_ptr<ma_encoder> encoder;

    /* Loopback mode is currently only supported on WASAPI. */
    ma_backend backends[1] = { ma_backend_wasapi };

};

}