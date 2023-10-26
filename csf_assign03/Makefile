CONSERVATIVE_FLAGS = -std=c++11 -Wall -Wextra -pedantic
DEBUGGING_FLAGS = -g -O0
CFLAGS = $(CONSERVATIVE_FLAGS) $(DEBUGGING_FLAGS)

csim: main.o 
	g++ -o csim main.o 

main.o: main.cpp Cache.h cache.cpp set.cpp Set.h Slot.h
	g++ -c main.cpp $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o main?

