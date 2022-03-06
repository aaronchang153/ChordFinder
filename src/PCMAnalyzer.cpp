#include "PCMAnalyzer.h"
#include <cmath>
#include <cstring>

#include "AudioWrapper.h" //needed for sample rate


namespace ChordFinder
{

const int PCMAnalyzer::outputDataLen = 88;

PCMAnalyzer::PCMAnalyzer(int frameSize)
{
    fft_out = nullptr;
    initialized = false;
    output_data.resize(outputDataLen);
    frames_per_buffer = 2;
    maxIdx = 0;
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
    global_frame_size = newSize;

    bufferSize = global_frame_size * frames_per_buffer;
    numBins = (bufferSize / 2);
    pcm_buffer = std::make_unique<double[]>(bufferSize);
    fft_out = std::make_unique<fftw_complex[]>(bufferSize);
    memset(pcm_buffer.get(), 0, sizeof(double) * bufferSize);
    memset(fft_out.get(), 0, sizeof(fftw_complex) * bufferSize);
    freq_data.resize(numBins);
    updateFFTWPlan();
}

void PCMAnalyzer::execute(std::vector<double> &data)
{
    { //shift left by 1 PCM frame
        double *buff = pcm_buffer.get();
        for(int i = 0; i < frames_per_buffer-1; i++)
        {
            memcpy(buff+(global_frame_size*i), buff+(global_frame_size*(i+1)), sizeof(double)*global_frame_size);
        }
        memcpy(buff+(global_frame_size*(frames_per_buffer-1)), data.data(), sizeof(double)*global_frame_size);
    }
    fftw_execute(plan);
    for(int i = 0; i < numBins; i++)
    {
        freq_data[i] = std::hypot(fft_out[i][0], fft_out[i][1]);
    }

    toNotes();

    maxIdx = 0;
    localMaxima.clear();
    for(unsigned int i = 1; i < output_data.size() - 1; i++)
    {
        if(output_data[i] > output_data[i-1] && output_data[i] > output_data[i+1])
        {
            localMaxima.push_back(static_cast<int>(i));
        }
        if(output_data[i] > output_data[maxIdx])
        {
            maxIdx = i;
        }
    }
}

void PCMAnalyzer::toNotes()
{
    NoteLibrary::Note_t currentNote; //Starts at A0 on construction
    int outputIdx = 0;
    float max = 0.0f;
    for(int i = 1; i < numBins; i++)
    {
        float freq = (static_cast<float>(AudioWrapper::sample_rate) / static_cast<float>(numBins)) * static_cast<float>(i);
        if(freq < currentNote.freqUB())
        {
            if(freq_data[i] > max)
            {
                max = freq_data[i];
            }
        }
        else
        {
            output_data[outputIdx++] = max;
            currentNote.getNextNote();
            max = freq_data[i];
        }
    }
}

void PCMAnalyzer::getData(std::vector<float> &dst)
{
    dst = output_data;
}

int PCMAnalyzer::getMax()
{
    return maxIdx;
}

std::vector<int> PCMAnalyzer::getLocalMaxima()
{
    return localMaxima;
}

void PCMAnalyzer::updateFFTWPlan()
{
    if(initialized)
    {
        fftw_destroy_plan(plan);
    }
    //TODO: need to set fftw flags?
    plan = fftw_plan_dft_r2c_1d(bufferSize, pcm_buffer.get(), fft_out.get(), 0);
    initialized = true;
}
    
}