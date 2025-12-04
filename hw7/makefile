make:
	mkdir -p bin
	g++ --std=c++17 -Wall -Wextra -pthread \
	    src/main.cpp \
	    -I include -o bin/search
run:
	./bin/search "<thread>" 
clean:
	rm -rf bin/search