CXX = g++
CXXFLAGS = -O2 -MD

SRC_OBJ = main.o driver.o Node.o Symtab.o Scanner.o
PARSER_OBJ = lex.yy.o compiler.tab.o

RMFILES = lex.yy.cc compiler.tab.cc compiler.tab.hh stack.hh *.d *.o location.hh position.hh
RMEXEC = out

TEST_SCRIPT = launch_tests.sh

.PHONY: clean all

all: out

# Compile the program and launch tests
tests: out
	./${TEST_SCRIPT}

# Compile the program
out: parse ${PARSER_OBJ} ${SRC_OBJ}
	${CXX} -o $@ ${SRC_OBJ} ${PARSER_OBJ}

parse: compiler.y scanner.l
	bison -d compiler.y
	flex scanner.l

lex.yy.o: lex.yy.cc
	${CXX} -c -o $@ $< ${CXXFLAGS}

compiler.tab.o: compiler.tab.cc
	${CXX} -c -o $@ $< ${CXXFLAGS}	

%.o: %.cc
	${CXX} -c -o $@ $< ${CXXFLAGS}

# Remove flex & bison output files and compilation files
clean:
	rm -rf ${RMEXEC} ${RMFILES}

-include *.d