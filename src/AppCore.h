#pragma once

#include "AudioWrapper.h"
#include "AudioQueue.h"

#include "imgui.h"

#include <cstdio>
#include <memory>
#include <thread>


namespace ChordFinder
{

class AppCore
{
public:
    AppCore();

    void start();
    void showWindows();
    void shutdown();

private:
    std::unique_ptr<AudioWrapper> audioWrapper;
    std::shared_ptr<AudioQueue> audioQueue; //should this be weak_ptr?
    std::shared_ptr<PCMAnalyzer> pcmAnalyzer;

    std::vector<char*> devices;

    ImGuiWindowFlags mainWindowFlags, configWindowFlags;

    void initialize();

    void showMainWindow();
    void showConfigWindow();
};

}