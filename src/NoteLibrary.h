#pragma once

#include <string>
#include <array>
#include <cmath>


namespace ChordFinder
{

class NoteLibrary
{
public:
    struct Note_t;

    static const std::array<std::string, 12> sharps; //From C to B
    static const std::array<std::string, 12> flats;  //From C to B

    static const std::array<float, 88> noteFrequency; //From A0 to C8

    static const float halfSemitoneUp;
    static const float halfSemitoneDown;

    static inline std::string noteIdxToStr(int idx, bool useFlats=false)
    {
        int i = (idx + 9) % 12;
        return useFlats ? flats[i] : sharps[i];
    }
};

struct NoteLibrary::Note_t
{
public:
    Note_t()
    {
        reset();
    }

    void reset()
    {
        //Start at A0
        note_num = 9;
        octave = 0;
    }

    inline float getFrequency()
    {
        //(octave * 12) because 12 semitones per octave
        //(note_num - 9) because note indexing starts at C, but the lowest note on the piano is an A (9 semitones up)
        return noteFrequency[(octave * 12) + (note_num - 9)];
    }

    //Returns the frequency half a semitone below this note
    inline float freqLB()
    {
        return getFrequency() * halfSemitoneDown;
    }

    //Returns the frequency half a semitone above this note
    inline float freqUB()
    {
        return getFrequency() * halfSemitoneUp;
    }

    inline void getNextNote()
    {
        note_num = (note_num + 1) % 12;
        if(note_num == 0)
        {
            octave++;
        }
    }

    inline std::string getName(bool useFlats = false)
    {
        return useFlats ? flats[note_num] : sharps[note_num];
    }

private:
    int note_num; //0-11: C to B in each octave
    int octave;
};

}