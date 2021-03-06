#SOURCES = $(wildcard *.cpp)
SOURCES = define.cpp utility.cpp bitboard.cpp itr.cpp board.cpp transptable.cpp infoboard.cpp \
	  search.cpp targetsearch.cpp nn.cpp learn.cpp brain.cpp inout.cpp test.cpp go.cpp tsumego.cpp fir.cpp main.cpp
OBJECTS = $(SOURCES:.cpp=.o)
CC = gcc -g -gdwarf-2 -Wno-deprecated
#CC = g++

include $(SOURCES:.cpp=.d)

main: $(OBJECTS)
	$(CC) -lstdc++ -lm -o $@ $^
.cpp.o: 
	$(CC) -c $< 
%.d: %.cpp
	$(CC) -M $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

clean:
	-rm *.o *.d *~ core.*
