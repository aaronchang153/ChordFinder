#include "AppCore.h"


namespace ChordFinder
{

AppCore::AppCore()
{
    audioWrapper = std::make_unique<AudioWrapper>();
    initialize();
}

AppCore::~AppCore()
{
    shutdown();
}

void AppCore::initialize()
{
    devices.push_back("default");
    mainWindowFlags = 0;
    configWindowFlags = 0;
}

void AppCore::start()
{
    audioWrapper->start();
    audioWrapper->startPCMAnalysis(std::ref(freqData), std::ref(freqDataMutex));
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
    { //Critical section
        std::unique_lock<std::mutex> lock(freqDataMutex);
        ImGui::PlotLines("PCM Out", freqData.data(), freqData.size(), 0, NULL, -1.0f, 1.0f, ImVec2(0, 160.0f));
    }
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