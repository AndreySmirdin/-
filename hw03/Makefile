CXX = g++
CXXFLAGS = -O2 -Wall -Werror -std=c++11 -I src -I test

all: main
main: bin huffman 
test: bin test.exe

huffman: bin/main.o bin/huffman.o 
	$(CXX) bin/main.o bin/huffman.o -o huffman
bin/main.o: src/main.cpp src/huffman.h 
	$(CXX) -c src/main.cpp $(CXXFLAGS) -o bin/main.o
bin/huffman.o: src/huffman.cpp src/huffman.h 
	$(CXX) -c src/huffman.cpp $(CXXFLAGS) -o bin/huffman.o


	
test.exe: bin/test.o bin/huffman_test.o  bin/huffman.o 
	$(CXX) bin/test.o bin/huffman_test.o bin/huffman.o  -o test.exe
bin/test.o: test/test.cpp test/huffman_test.h src/huffman.h 
	$(CXX) -c test/test.cpp $(CXXFLAGS) -o bin/test.o
bin/huffman_test.o: test/huffman_test.cpp test/huffman_test.h src/huffman.h
	$(CXX) -c test/huffman_test.cpp $(CXXFLAGS) -o bin/huffman_test.o
  
bin:
	mkdir -p bin
clean:
	rm -r -f bin
	rm huffman
	rm test.exe 
