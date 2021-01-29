CPP =g++
CPPFLAGS = -Wall
all:
	${CPP} set.cpp -o set -DCOMPARE ${CPPFLAGS}
avl:
	${CPP} set.cpp -o set -DAVL ${CPPFLAGS}
tests:
	${CPP} testmaker.cpp -o testmaker ${CPPFLAGS}
gtests:
	${CPP} set.cpp -o set -DGTESTS -lgtest -pthread ${CPPFLAGS}