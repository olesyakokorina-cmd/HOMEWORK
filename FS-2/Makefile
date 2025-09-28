CXX = g++
CXXFLAGS = -Wall -Wextra -O2 -std=c++11

TARGET = simple-copy
OBJ = simple-copy.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

$(OBJ): simple-copy.cpp
	$(CXX) $(CXXFLAGS) -c simple-copy.cpp -o $(OBJ)

clean:
	rm -f $(TARGET) $(OBJ)
