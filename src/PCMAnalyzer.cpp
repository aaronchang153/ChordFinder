#include "PCMAnalyzer.h"
#include <cmath>
#include <cstring>


namespace ChordFinder
{

PCMAnalyzer::PCMAnalyzer(int frameSize)
{
    fft_out = nullptr;
    initialized = false;
    setFrameSize(frameSize);
}

PCMAnalyzer::~PCMAnalyzer()
{
    if(initialized)
    {
        fftw_destroy_plan(plan);
    }
}

void PCMAnalyzer::setFrameSize(int newSize)
{
    frameSize = newSize;
    numBins = (frameSize / 2) + 1;
    frame.resize(frameSize);
    fft_out = std::make_unique<fftw_complex[]>(frameSize);
    freq_data.resize(numBins);
    updateFFTWPlan();
}

void PCMAnalyzer::execute(std::vector<double> &data)
{
    frame = data;
    //fftw_execute(plan);
    for(int i = 0; i < numBins; i++)
    {
        freq_data[i] = static_cast<float>(frame[i]*50);//std::hypot(fft_out[i][0], fft_out[i][1]));
    }
}

void PCMAnalyzer::getData(std::vector<float> &dst)
{
    dst = freq_data;
}

void PCMAnalyzer::updateFFTWPlan()
{
    if(initialized)
    {
        fftw_destroy_plan(plan);
    }
    //TODO: need to set fftw flags?
    plan = fftw_plan_dft_r2c_1d(frameSize, frame.data(), fft_out.get(), 0);
    initialized = true;
}
    
}