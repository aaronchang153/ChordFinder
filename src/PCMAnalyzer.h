#pragma once

#include "fftw/fftw3.h"
#include <vector>
#include <memory>
#include <thread>


namespace ChordFinder
{

class PCMAnalyzer
{
public:
    PCMAnalyzer(int);
    ~PCMAnalyzer();

    void execute(std::vector<double>&);
    void getData(std::vector<float>&);

private:
    int frameSize; //the amount of data in each PCM frame
    int numBins; //the number of bins in the final freq output data

    bool initialized;

    std::vector<double> frame;
    std::unique_ptr<fftw_complex[]> fft_out;
    fftw_plan plan;

    std::vector<float> freq_data;
    std::array<float, 88> output_data;

    void changeFrameSize(int);
    void updateFFTWPlan();
};

}