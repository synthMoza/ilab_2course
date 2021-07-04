CXX = g++
CXXFLAGS = -lgtest -pthread -MD
RMEXEC = matrix gtests tests
RMFILES = *.d *.o

.PHONY: all clean

all: matrix

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

matrix: matrix.o
	$(CXX) -o $@ $< $(CXXFLAGS)

tests: testGenerator.o
	$(CXX) -o $@ $< $(CXXFLAGS)

gtests: matrixg.o
	$(CXX) -o $@ $< $(CXXFLAGS) -DGTESTS

matrixg.o: matrix.cpp
	$(CXX) -c -o $@ $< $(CFLAGS) -DGTESTS

clean:
	rm -rf $(RMEXEC) $(RMFILES)

-include *.d