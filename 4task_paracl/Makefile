CXX = g++
CXXFLAGS = -O2 -MD

SRC_OBJ = main.o driver.o Node.o Symtab.o
PARSER_OBJ = lex.yy.o compiler.tab.o

RMFILES = lex.yy.cc compiler.tab.cc compiler.tab.hh stack.hh *.d *.o location.hh position.hh
RMEXEC = out

.PHONY: clean all

all: out

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

clean:
	rm -rf ${RMEXEC} ${RMFILES}

-include *.d