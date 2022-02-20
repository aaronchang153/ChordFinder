CC=g++
CFLAGS=-Ilib -Wall -c -g
LFLAGS=-lmingw32

%.o: %.cpp
	$(CC) $(CFLAGS) -o $@ $<

target : src/AudioWrapper.o src/main.o
	$(CC) $(LFLAGS) -o $@ $^