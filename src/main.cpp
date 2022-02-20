#include "AudioWrapper.h"
#include <windows.h>

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevIns, LPSTR lpszArgument, int iShow)
{
    ChordFinder::AudioWrapper audioWrapper;
    audioWrapper.test();
    return 0;
}