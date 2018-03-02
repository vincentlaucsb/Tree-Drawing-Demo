CXX = clang++
CFLAGS = -lpthread -std=c++11 -Wall -O3
SOURCES = src/tree.cpp src/tree_draw.cpp
BINARIES = tree

all: $(BINARIES)

tree:
	$(CXX) -o tree $(SOURCES) src/main.cpp -Ilib/ $(CFLAGS)
	
runtimeTest:
	$(CXX) -o runtimeTest $(SOURCES) src/runtimeTest.cpp -Ilib/ -lpthread -std=c++11 -O3

clean:
	rm -rf $(BINARIES)

.PHONY: all