CXX = g++
CXXFLAGS = -std=c++11 -Wall
TARGET = interactive-shell

all: $(TARGET)

$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.cpp

clean:
	rm -f $(TARGET) *.log

.PHONY: all clean
