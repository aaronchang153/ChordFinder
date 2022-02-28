#pragma once

#include "fftw/fftw3.h"
#include <memory>
#include <thread>


namespace ChordFinder
{

class PCMAnalyzer
{
public:
    PCMAnalyzer();
    ~PCMAnalyzer();

    void changeFrameSize(int);

    void execute(double*);
    void getData(float*);

private:
    int frameSize; //the amount of data in each PCM frame
    int numBins; //the number of bins in the final freq output data

    std::unique_ptr<double[]> frame;
    std::unique_ptr<fftw_complex[]> fft_out;
    fftw_plan plan;

    std::unique_ptr<float[]> freq_data;
    std::array<float, 88> output_data;

    void updateFFTWPlan();
};

}