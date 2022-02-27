#include "AudioQueue.h"


namespace ChordFinder
{

AudioQueue::AudioQueue(int numFrames, int frameSize)
{
    this->numFrames = numFrames;
    this->frameSize = frameSize;
    for(int i = 0; i < numFrames; i++)    
    {
        framePool.push(std::make_unique<double[]>(frameSize));
    }
    getNewFrame();
}

void AudioQueue::enqueue(const float *data, int len)
{
    for(int i = 0; i < len; i++)
    {
        if(framePtr < frameSize)
        {
            currentFrame[framePtr++] = static_cast<double>(data[i]);
        }
        else
        {
            //We have a full frame
            outputFrames.push(std::move(currentFrame));
            getNewFrame();
        }
    }
}

std::unique_ptr<double[]> AudioQueue::dequeue()
{
    if(outputFrames.empty())
    {
        return nullptr;
    }
    std::unique_ptr<double[]> tmp = std::move(outputFrames.front());
    outputFrames.pop();
    return tmp;
}

void AudioQueue::release(std::unique_ptr<double[]> &ptr)
{
    framePool.push(std::move(ptr));
}

void AudioQueue::getNewFrame()
{
    //TODO: handle case where framePool is empty?
    currentFrame = std::move(framePool.front());
    framePool.pop();
    framePtr = 0;
}

}