#include "AppCore.h"
#include "imgui.h"


namespace ChordFinder
{

AppCore::AppCore()
{
    audioWrapper = std::make_unique<AudioWrapper>();
    audioQueue = audioWrapper->getAudioQueue();
    audioThread = nullptr;
}

void AppCore::start()
{
    audioThread = std::make_unique<std::thread>(threadproc, audioWrapper.get());
}

void AppCore::showMainWindow()
{
    static float values[0x1000] = {};
    static int values_offset = 0;
    //static double refresh_time = 0.0;
    //while (refresh_time < ImGui::GetTime()) // Create data at fixed 60 Hz rate for the demo
    //{
    //    static float phase = 0.0f;
    //    values[values_offset] = cosf(phase);
    //    values_offset = (values_offset + 1) % IM_ARRAYSIZE(values);
    //    phase += 0.10f * values_offset;
    //    refresh_time += 1.0f / 60.0f;
    //}
    std::unique_ptr<double[]> ptr;
    do
    {
        ptr = audioQueue->dequeue();
    } while (ptr == nullptr);
    

    // Plots can display overlay texts
    // (in this example, we will display an average value)
    {
        for(int i = 0; i < 0x1000; i++)
        {
            values[i] = static_cast<float>(ptr[i]*50);
        }
        ImGui::PlotLines("PCM Out", values, IM_ARRAYSIZE(values), values_offset, NULL, -1.0f, 1.0f, ImVec2(0, 160.0f));
    }
    audioQueue->release(ptr);
}

void AppCore::threadproc(AudioWrapper *awrap)
{
    awrap->test();
}

}