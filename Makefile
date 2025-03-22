CXX = g++
CXXFLAGS = -std=c++11 -Wall -Iinclude
SRC = src/InsertionSort.cpp src/MergeSort.cpp src/SelectionSort.cpp src/BubbleSort.cpp src/HeapSort.cpp main.cpp
OBJ = $(SRC:.cpp=.o)
EXEC = main

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)