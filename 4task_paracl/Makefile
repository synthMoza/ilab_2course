CXX = g++
CXXFLAGS = -O2 -MD

SRC_OBJ = main.o driver.o Node.o INode.o Symtab.o
PARSER_OBJ = lex.yy.o grammar.tab.o

RMFILES = lex.yy.cc grammar.tab.cc grammar.tab.hh stack.hh *.d *.o location.hh position.hh
RMEXEC = out

.PHONY: clean all

all: out

out: parse ${PARSER_OBJ} ${SRC_OBJ}
	${CXX} -o $@ ${SRC_OBJ} ${PARSER_OBJ}

parse: grammar.y grammar.l
	bison -d grammar.y
	flex grammar.l

lex.yy.o: lex.yy.cc
	${CXX} -c -o $@ $< ${CXXFLAGS}

grammar.tab.o: grammar.tab.cc
	${CXX} -c -o $@ $< ${CXXFLAGS}	

%.o: %.cc
	${CXX} -c -o $@ $< ${CXXFLAGS}

clean:
	rm -rf ${RMEXEC} ${RMFILES}

-include *.d