#pragma once

#include "AudioWrapper.h"
#include "AudioQueue.h"

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
    void showMainWindow();

private:
    std::unique_ptr<AudioWrapper> audioWrapper;
    std::shared_ptr<AudioQueue> audioQueue; //should this be weak_ptr?

    std::unique_ptr<std::thread> audioThread;

    static void threadproc(AudioWrapper*);
};

}