#include "PCMAnalyzer.h"
#include <cmath>
#include <cstring>


namespace ChordFinder
{

PCMAnalyzer::PCMAnalyzer()
{
    frameSize = 0;
    frame = nullptr;
    fft_out = nullptr;
    freq_data = nullptr;
}

PCMAnalyzer::~PCMAnalyzer()
{
    if(fft_out != nullptr)
    {
        fftw_destroy_plan(plan);
    }
}

void PCMAnalyzer::changeFrameSize(int newSize)
{
    frameSize = newSize;
    numBins = (frameSize / 2) + 1;
    frame = std::make_unique<double[]>(frameSize);
    fft_out = std::make_unique<fftw_complex[]>(frameSize);
    freq_data = std::make_unique<float[]>(numBins);
    updateFFTWPlan();
}

void PCMAnalyzer::execute(double *data)
{
    memcpy(frame.get(), data, sizeof(double) * frameSize);
    fftw_execute(plan);
    for(int i = 0; i < numBins; i++)
    {
        freq_data[i] = static_cast<float>(std::hypot(fft_out[i][0], fft_out[i][1]));
    }
}

void PCMAnalyzer::getData(float *dst)
{
    memcpy(dst, freq_data.get(), sizeof(float) * frameSize);
}

void PCMAnalyzer::updateFFTWPlan()
{
    if(fft_out != nullptr)
    {
        fftw_destroy_plan(plan);
    }
    //TODO: need to set fftw flags?
    plan = fftw_plan_dft_r2c_1d(frameSize, frame.get(), fft_out.get(), 0);
}
    
}