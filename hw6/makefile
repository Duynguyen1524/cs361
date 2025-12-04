make:
	 mkdir -p bin
	 g++ --std=c++17 -Wall -Wextra -pthread \
	     src/main.cpp src/Boat.cpp src/semaphore.cpp \
	     -I include -o bin/island

run:
	 ./bin/island 7 9

clean:
	 rm -rf bin
