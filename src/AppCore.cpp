#include "AppCore.h"


namespace ChordFinder
{

AppCore::AppCore()
{
    audioWrapper = std::make_unique<AudioWrapper>();
    audioQueue = audioWrapper->getAudioQueue();
    pcmAnalyzer = audioWrapper->getPCMAnalyzer();
    initialize();
}

void AppCore::initialize()
{
    devices.push_back("default");
    mainWindowFlags = 0;
    configWindowFlags = 0;
}

void AppCore::start()
{
    audioWrapper->start(); //TODO: does miniaudio audio capture happen in another thread by default?
}

void AppCore::shutdown()
{
    audioWrapper->shutdown();
}

void AppCore::showWindows()
{
    showMainWindow();
    showConfigWindow();
}

void AppCore::showMainWindow()
{
    ImGui::Begin("Audio View", NULL, mainWindowFlags);
    static float values[0x1000] = {};
    static int values_offset = 0;
    std::unique_ptr<double[]> ptr;
    int timeout = 50;
    do
    {
        ptr = audioQueue->dequeue();
    } while ((ptr == nullptr) && (--timeout > 0));
    
    if(ptr != nullptr)
    {
        for(int i = 0; i < 0x1000; i++)
        {
            values[i] = static_cast<float>(ptr[i]*50);
        }
        audioQueue->release(ptr);
    }
    ImGui::PlotLines("PCM Out", values, IM_ARRAYSIZE(values), values_offset, NULL, -1.0f, 1.0f, ImVec2(0, 160.0f));
    ImGui::End();
}

void AppCore::showConfigWindow()
{
    ImGui::Begin("Configuration", NULL, configWindowFlags);

    // Using the _simplified_ one-liner Combo() api here
    // See "Combo" section for examples of how to use the more flexible BeginCombo()/EndCombo() api.
    static int item_current = 0;
    ImGui::Combo("Audio Device", &item_current, devices.data(), devices.size());

    const char* items2[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIIIIII", "JJJJ", "KKKKKKK" };
    static int item_current2 = 0;
    ImGui::Combo("combo2", &item_current2, items2, IM_ARRAYSIZE(items2));

    ImGui::End();
}

}