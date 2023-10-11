CONSERVATIVE_FLAGS = -std=c++11 -Wall -Wextra -pedantic
DEBUGGING_FLAGS = -g -O0
CFLAGS = $(CONSERVATIVE_FLAGS) $(DEBUGGING_FLAGS)

main: main.o 
	g++ -o main main.o 

main.o: main.cpp # include main.h later if needed
	g++ -c main.cpp $(CFLAGS)

.PHONY: clean
clean:
	rm -f *.o main?
