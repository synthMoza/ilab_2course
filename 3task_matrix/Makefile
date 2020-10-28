CC=g++
CFLAGS = -lgtest -pthread

all:
	@ $(CC) -o matrix matrix.cpp $(CFLAGS)
tests:
	@ $(CC) -o tests testGenerator.cpp $(CFLAGS)
gtests:
	@ $(CC) -o matrix matrix.cpp $(CFLAGS) -DGTESTS
