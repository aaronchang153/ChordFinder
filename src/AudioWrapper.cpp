#define MINIAUDIO_IMPLEMENTATION
#include "AudioWrapper.h"

namespace ChordFinder
{

static void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    //ma_encoder* pEncoder = (ma_encoder*)pDevice->pUserData;
    //MA_ASSERT(pEncoder != NULL);

    //ma_encoder_write_pcm_frames(pEncoder, pInput, frameCount, NULL);
    AudioQueue *queue = (AudioQueue *)pDevice->pUserData;
    queue->enqueue((const float *)pInput, frameCount);

    (void)pOutput;
}

AudioWrapper::AudioWrapper()
{
    device = std::make_unique<ma_device>();
    encoder = std::make_unique<ma_encoder>();
    aqueue = std::make_shared<AudioQueue>(0x1000);
    analyzer = std::make_unique<PCMAnalyzer>();
}

AudioWrapper::~AudioWrapper()
{
}

void AudioWrapper::start()
{

}

void AudioWrapper::test()
{
    ma_result result;
    ma_encoder_config encoderConfig;
    ma_device_config deviceConfig;

    encoderConfig = ma_encoder_config_init(ma_encoding_format_wav, ma_format_f32, channels, sample_rate);

    if (ma_encoder_init_file("output.wav", &encoderConfig, encoder.get()) != MA_SUCCESS) {
        printf("Failed to initialize output file.\n");
        return;
    }

    deviceConfig = ma_device_config_init(ma_device_type_loopback);
    deviceConfig.capture.pDeviceID = NULL; /* Use default device for this example. Set this to the ID of a _playback_ device if you want to capture from a specific device. */
    deviceConfig.capture.format    = encoder->config.format;
    deviceConfig.capture.channels  = encoder->config.channels;
    deviceConfig.sampleRate        = encoder->config.sampleRate;
    deviceConfig.dataCallback      = ChordFinder::data_callback;
    deviceConfig.pUserData         = aqueue.get();//encoder.get();

    result = ma_device_init_ex(backends, sizeof(backends)/sizeof(backends[0]), NULL, &deviceConfig, device.get());
    if (result != MA_SUCCESS) {
        printf("Failed to initialize loopback device.\n");
        return;
    }

    result = ma_device_start(device.get());
    if (result != MA_SUCCESS) {
        ma_device_uninit(device.get());
        printf("Failed to start device.\n");
        return;
    }

    printf("Press Enter to stop recording...\n");
    getchar();
    
    ma_device_uninit(device.get());
    ma_encoder_uninit(encoder.get());
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