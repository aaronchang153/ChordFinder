#pragma once

#include "AudioWrapper.h"
#include "AudioQueue.h"

#include "imgui.h"

#include <cstdio>
#include <memory>
#include <thread>
#include <chrono>
#include <mutex>


namespace ChordFinder
{

class AppCore
{
public:
    AppCore();
    virtual ~AppCore();

    void start();
    void showWindows();
    void shutdown();

private:
    std::unique_ptr<AudioWrapper> audioWrapper;

    std::vector<char*> devices;

    std::vector<float> freqData;
    std::mutex freqDataMutex;

    ImGuiWindowFlags mainWindowFlags, configWindowFlags;

    void initialize();

    void showMainWindow();
    void showConfigWindow();
};

}