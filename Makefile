CXX = clang++
CFLAGS = -lpthread -std=c++11 -g
SOURCES = $(wildcard src/*.cpp)

all: test

test:
	$(CXX) -o test $(SOURCES) -Ilib/ $(CFLAGS)

clean:
	rm -rf test

.PHONY: all