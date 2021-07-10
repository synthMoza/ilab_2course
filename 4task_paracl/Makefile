CXX = g++
CXXFLAGS = -O2
SRC = grammar.tab.cc lex.yy.cc driver.cc Node.cc INode.cc Symtab.cc
RMFILES = lex.yy.cc grammar.tab.cc grammar.tab.hh stack.hh
RMEXEC = out

.PHONY: clean all

all: out

out: driver.cc Node.cc INode.cc grammar.y grammar.l driver.hpp
	bison -d grammar.y
	flex grammar.l
	${CXX} ${SRC} -o $@ ${CXXFLAGS}

clean:
	rm -rf ${RMEXEC} ${RMFILES}