#pragma once

#include <queue>
#include <vector>
#include <memory>

#include <cstdio>


namespace ChordFinder
{

class AudioQueue
{
public:
    AudioQueue(int);

    void enqueue(const float*, int);
    std::unique_ptr<std::vector<double>> dequeue();

    void release(std::unique_ptr<std::vector<double>>&);

    int getFrameSize() { return frameSize; }
    void setFrameSize(int newSize) { frameSize = newSize; } //TODO: do I actually want to allow this to change dynamically?

private:
    const int DEFAULT_NUM_FRAMES = 10; //init with this many, but it can expand

    int frameSize;

    std::queue<std::unique_ptr<std::vector<double>>> framePool;
    std::unique_ptr<std::vector<double>> currentFrame;

    std::queue<std::unique_ptr<std::vector<double>>> outputFrames;

    void getNewFrame();
};

}