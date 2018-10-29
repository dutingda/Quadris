CXX=g++
CXXFLAGS=-std=c++14 -Wextra -Wpedantic -Wall -MMD -g
OBJECTS=block.o difficulty.o levelzero.o levelone.o leveltwo.o levelthree.o levelfour.o grid.o blockholder.o textdisplay.o hintgenerator.o score.o graphicsdisplay.o window.o main.o
DEPENDS=${OBJECTS:.o=.d}
EXEC=quadris
TEST_SUITE=quadris.txt

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC} -lX11

-include ${DEPENDS}

clean:
	mv *~ backup-src
	rm ${OBJECTS} ${DEPENDS} ${EXEC}
	rm *.zip
.PHONY: clean

zip:
	zip ${EXEC}.zip *
.PHONY: zip

run:
	make
	./${EXEC}
.PHONY: run

valgrind:
	make
	valgrind ./${EXEC}
.PHONY: valgrind
