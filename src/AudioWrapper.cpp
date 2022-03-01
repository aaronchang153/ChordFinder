#define MINIAUDIO_IMPLEMENTATION
#include "AudioWrapper.h"

namespace ChordFinder
{

AudioWrapper::AudioWrapper()
{
    device = std::make_unique<ma_device>();
    aqueue = std::make_shared<AudioQueue>(0x1000);
    analyzer = std::make_unique<PCMAnalyzer>();

    deviceActive = false;
}

AudioWrapper::~AudioWrapper()
{
}

void AudioWrapper::start()
{
    initializeDevice();
    startDevice();
}

void AudioWrapper::shutdown()
{
    stopDevice();
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

std::shared_ptr<AudioQueue> AudioWrapper::getAudioQueue()
{
    return aqueue;
}

std::shared_ptr<PCMAnalyzer> AudioWrapper::getPCMAnalyzer()
{
    return analyzer;
}

}