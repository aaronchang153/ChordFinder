#pragma once

#include <queue>
#include <memory>

#include <cstdio>


namespace ChordFinder
{

class AudioQueue
{
public:
    AudioQueue(int);

    void enqueue(const float*, int);
    std::unique_ptr<double[]> dequeue();

    void release(std::unique_ptr<double[]>&);

    int getFrameSize() { return frameSize; }

private:
    const int DEFAULT_NUM_FRAMES = 10; //init with this many, but it can expand

    int frameSize;
    int framePtr;

    std::queue<std::unique_ptr<double[]>> framePool;
    std::unique_ptr<double[]> currentFrame;

    std::queue<std::unique_ptr<double[]>> outputFrames;

    void getNewFrame();
};

}