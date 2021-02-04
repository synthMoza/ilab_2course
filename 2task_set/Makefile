CXX = g++
CXXFLAGS = -O2 -Wall -MD
GCXXFLAGS = -DGTESTS -lgtest -pthread

all: compare

# Compare the amount of time needed to comlete the task between
# AVL tree and std::set
compare: set.o compare.o
	$(CXX) -o $@ $^
compare.o: compare.cpp
	$(CXX) -c -o $@ $< -DCOMPARE $(CXXFLAGS)

# Tests to ensure AVL tree works right
gtests: set.o gcompare.o
	$(CXX) -o $@ $^ $(GCXXFLAGS)
gcompare.o: compare.cpp
	$(CXX) -c -o $@ $< $(GCXXFLAGS) $(CXXFLAGS)

# Create tests
testmaker: testmaker.o
	$(CXX) -o $@ $^
	./$@
testmaker.o: testmaker.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

# Clean garbage
clean:
	rm -rf compare gtests testmaker *.o *.d 
