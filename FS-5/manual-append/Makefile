CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++11

TARGET := manual-append
SRC := main.cpp
OBJ := main.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJ)
