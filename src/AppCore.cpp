#include "AppCore.h"


namespace ChordFinder
{

static void HelpMarker(const char* desc);

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
    char temp[] = "defualt";
    devices.push_back(temp);
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
    auto winSize = ImGui::GetWindowSize();
    { //Critical section
        std::unique_lock<std::mutex> lock(freqDataMutex);
        int maxIdx;
        float maxValue;
        try
        {
            maxIdx = audioWrapper->getMaxOutputIndex();
            maxValue = freqData.at(maxIdx) * 1.05; //add a small buffer for aesthetics
        }
        catch(const std::out_of_range&)
        {
            //Not really an error. There just isn't any audio data yet.
            maxValue = 0.0f;
        }

        if(maxValue < 0.5f) { maxValue = 0.5f; }
        ImGui::PlotHistogram("", freqData.data(), freqData.size(), 0, NULL, -maxValue*0.1f, maxValue, ImVec2(winSize.x-15, winSize.y/2));

        ImGui::Text("Max Value: %.2f", maxValue);
        ImGui::SameLine(0.0f, 0.0f);
        HelpMarker("A low maximum value may indicate that the results are just noise");

        ImGui::Text("Loudest Note: %s", NoteLibrary::noteIdxToStr(maxIdx).c_str());
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

// Helper to display a little (?) mark which shows a tooltip when hovered.
// In your own code you may want to display an actual icon if you are using a merged icon fonts (see docs/FONTS.md)
static void HelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

}