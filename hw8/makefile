CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic -pthread -O2

TARGET   := bin/sequence
SRCS     := main.cpp
OBJS     := $(SRCS:.cpp=.o)

.PHONY: all clean test7 test9 test11 test13

all: $(TARGET)

bin:
	mkdir -p bin

$(TARGET): bin $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

test7: $(TARGET)
	./bin/sequence 7 seq7.txt

test9: $(TARGET)
	./bin/sequence 9 seq9.txt

test11: $(TARGET)
	./bin/sequence 11 seq11.txt

test13: $(TARGET)
	./bin/sequence 13 seq13.txt

clean:
	rm -f $(OBJS) $(TARGET)
