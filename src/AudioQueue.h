#pragma once

#include <queue>
#include <memory>

#include <cstdio>


namespace ChordFinder
{

class AudioQueue
{
public:
    AudioQueue(int, int);

    void enqueue(const float*, int);
    std::unique_ptr<double[]> dequeue();

    void release(std::unique_ptr<double[]>&);

private:
    int numFrames;
    int frameSize;
    int framePtr;

    std::queue<std::unique_ptr<double[]>> framePool;
    std::unique_ptr<double[]> currentFrame;

    std::queue<std::unique_ptr<double[]>> outputFrames;

    void getNewFrame();
};

}