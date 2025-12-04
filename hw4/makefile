all:
	mkdir -p bin
	g++ --std=c++17 -o  bin/race  src/race.cpp src/semaphore.cpp
test1:
	./bin/race 3

test2:
	./bin/race 5

test3:
	./bin/race 10
clean:
	rm -rf bin