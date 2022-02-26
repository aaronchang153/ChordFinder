CC=g++
CFLAGS=-Ilib -Wall -c -g
LFLAGS=-lmingw32
DLL=lib/fftw/libfftw3-3.dll

%.o: %.cpp
	$(CC) $(CFLAGS) -o $@ $<

target : src/AudioWrapper.o src/AudioQueue.o src/main.o
	$(CC) $(LFLAGS) -o $@ $^ $(DLL)