CC   = g++ -std=c++11 -W -Iinclude
OPTS = -Wall -O3 -lpthread
.SUFFIXES: .cpp
.cpp:
	$(CC) $(OPTS) $<
all:
	make terra
	make lunar
	make amateras
terra:
	$(CC) src/terra.cpp $(OPTS) -o bin/terra
lunar:
	$(CC) src/lunar.cpp $(OPTS) -o bin/lunar
amateras:
	$(CC) src/amaterachu.cpp -O2 -o  bin/amaterachu
