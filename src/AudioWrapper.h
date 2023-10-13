#pragma once

#include "miniaudio/miniaudio.h"
#include <fftw3.h>

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

    int getMaxOutputIndex();

    static const ma_format format;
    static const ma_uint32 channels;
    static const ma_uint32 sample_rate;

    static const int FRAME_SIZE;

private:
    std::unique_ptr<ma_device> device;

    std::shared_ptr<AudioQueue> aqueue;
    std::shared_ptr<PCMAnalyzer> analyzer;

    /* Loopback mode is currently only supported on WASAPI. */
    ma_backend backends[1] = { ma_backend_wasapi };

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