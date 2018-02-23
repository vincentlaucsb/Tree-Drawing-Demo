CXX = clang++
CFLAGS = -lpthread -std=c++11 -g
SOURCES = src/tree.cpp

all: test

test:
	$(CXX) -o test $(SOURCES) src/main.cpp -Ilib/ $(CFLAGS)
	
runtimeTest:
	$(CXX) -o runtimeTest $(SOURCES) src/runtimeTest.cpp -Ilib/ -lpthread -std=c++11 -O3

clean:
	rm -rf test

.PHONY: all