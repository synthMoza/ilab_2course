CXX = g++
CXXFLAGS = -Wall -O2 -MD

SRC_OBJ = main.o
RMFILES = *.o *.d
RMEXEC = out

.PHONY: clean all

all: out

out: ${SRC_OBJ}
	${CXX} -o $@ $< ${CXXFLAGS}

clean:
	rm -rf ${RMFILES} ${RMEXEC}