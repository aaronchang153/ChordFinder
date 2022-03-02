#define MINIAUDIO_IMPLEMENTATION
#include "AudioWrapper.h"
#include <cmath>

namespace ChordFinder
{

AudioWrapper::AudioWrapper()
{
    device = std::make_unique<ma_device>();
    aqueue = std::make_shared<AudioQueue>(FRAME_SIZE);
    analyzer = std::make_shared<PCMAnalyzer>(FRAME_SIZE);

    deviceActive = false;
    shuttingDown = false;
}

AudioWrapper::~AudioWrapper()
{
}

void AudioWrapper::start()
{
    initializeDevice();
    startDevice();
}

void AudioWrapper::startPCMAnalysis(std::vector<float> &dstVector, std::mutex &mutex)
{
    analysisThread = std::thread(AudioWrapper::pcm_analysis_proc, analyzer, aqueue,
                                 std::ref(dstVector), std::ref(mutex), std::ref(shuttingDown));
}

void AudioWrapper::shutdown()
{
    shuttingDown = true;
    stopDevice();
    if(analysisThread.joinable())
    {
        analysisThread.join();
    }
}

bool AudioWrapper::initializeDevice(ma_device_id *device_id)
{
    ma_result result;
    ma_device_config deviceConfig;

    stopDevice();

    deviceConfig = ma_device_config_init(ma_device_type_loopback);
    deviceConfig.capture.pDeviceID = device_id; /* Use default device for this example. Set this to the ID of a _playback_ device if you want to capture from a specific device. */
    deviceConfig.capture.format    = format;
    deviceConfig.capture.channels  = channels;
    deviceConfig.sampleRate        = sample_rate;
    deviceConfig.dataCallback      = AudioWrapper::data_callback;
    deviceConfig.pUserData         = aqueue.get();

    result = ma_device_init_ex(backends, sizeof(backends)/sizeof(backends[0]), NULL, &deviceConfig, device.get());
    if (result != MA_SUCCESS) {
        printf("Failed to initialize loopback device.\n");
        return false;
    }
    return true;
}

bool AudioWrapper::startDevice()
{
    ma_result result = ma_device_start(device.get());
    if (result != MA_SUCCESS) {
        ma_device_uninit(device.get());
        printf("Failed to start device.\n");
        return false;
    }
    deviceActive = true;
    return true;
}

void AudioWrapper::stopDevice()
{
    if(deviceActive)
    {
        ma_device_uninit(device.get());
        deviceActive = false;
    }
}

void AudioWrapper::data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    AudioQueue *queue = (AudioQueue *)pDevice->pUserData;
    queue->enqueue((const float *)pInput, frameCount);

    (void)pOutput;
}

void AudioWrapper::pcm_analysis_proc(std::shared_ptr<PCMAnalyzer> analyzer,
                                     std::shared_ptr<AudioQueue> queue,
                                     std::vector<float> &dataOut,
                                     std::mutex &mutex,
                                     bool &shutdown)
{
    int timeout;
    std::unique_ptr<std::vector<double>> frame;
    while(!shutdown)
    {
        do
        {
            frame = queue->dequeue();
        } while((frame == nullptr) && (--timeout > 0));

        if(frame != nullptr)
        {
            analyzer->execute(*frame);

            { //Critical section
                std::unique_lock<std::mutex> lock(mutex);
                if(dataOut.size() != frame->size())
                {
                    dataOut.resize(frame->size());
                }
                analyzer->getData(std::ref(dataOut));
            }

            queue->release(frame);
        }
    }
}

}