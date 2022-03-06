#pragma once

#include "fftw/fftw3.h"
#include <vector>
#include <memory>
#include <thread>

#include "NoteLibrary.h"


namespace ChordFinder
{

class PCMAnalyzer
{
public:
    PCMAnalyzer(int);
    virtual ~PCMAnalyzer();

    void execute(std::vector<double>&);
    void getData(std::vector<float>&);

    int getMax();
    std::vector<int> getLocalMaxima();

private:
    int global_frame_size; //the amount of data in each PCM frame
    
    int frames_per_buffer;
    int bufferSize; //the amount of PCM data that we're doing the FFT on
    int numBins; //the number of non-redundant bins in the FFT output

    bool initialized;

    static const int outputDataLen;

    std::unique_ptr<double[]> pcm_buffer;
    std::unique_ptr<fftw_complex[]> fft_out;
    fftw_plan plan;

    std::vector<float> freq_data;
    std::vector<float> output_data;

    //metadata on output_data
    int maxIdx;
    std::vector<int> localMaxima;

    void toNotes();

    void setFrameSize(int);
    void updateFFTWPlan();
};

}