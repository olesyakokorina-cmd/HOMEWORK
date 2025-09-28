CXX = g++
CXXFLAGS = -Wall -Wextra -O2 -std=c++11

TARGET = read-file
OBJ = read-file.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

$(OBJ): read-file.cpp
	$(CXX) $(CXXFLAGS) -c read-file.cpp -o $(OBJ)

clean:
	rm -f $(TARGET) $(OBJ)
