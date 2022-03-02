#pragma once

#include "miniaudio/miniaudio.h"
#include "fftw/fftw3.h"

#include <cstdio>
#include <memory>
#include <thread>
#include <chrono>
#include <mutex>

#include "AudioQueue.h"
#include "PCMAnalyzer.h"


namespace ChordFinder
{

class AudioWrapper
{
public:
    AudioWrapper();
    virtual ~AudioWrapper();

    void start();
    void startPCMAnalysis(std::vector<float>&, std::mutex&);
    void shutdown();

    //For filtering since I only care about the range A0-C8
    //static const float FREQ_LO = 27.5f;
    //static const float FREQ_HI = 4186.0f;

private:
    std::unique_ptr<ma_device> device;

    std::shared_ptr<AudioQueue> aqueue;
    std::shared_ptr<PCMAnalyzer> analyzer;

    /* Loopback mode is currently only supported on WASAPI. */
    ma_backend backends[1] = { ma_backend_wasapi };

    const ma_format format = ma_format_f32;
    const ma_uint32 channels = 1;
    const ma_uint32 sample_rate = 44100;

    const int FRAME_SIZE = 0x1000;

    std::thread analysisThread;

    //Internal State Variables
    bool deviceActive;
    bool shuttingDown;

    bool initializeDevice(ma_device_id * = NULL);
    bool startDevice();
    void stopDevice();

    static void data_callback(ma_device*, void*, const void*, ma_uint32);
    static void pcm_analysis_proc(std::shared_ptr<PCMAnalyzer> analyzer,
                                  std::shared_ptr<AudioQueue> queue,
                                  std::vector<float> &dataOut,
                                  std::mutex &mutex,
                                  bool &shutdown);

};

}