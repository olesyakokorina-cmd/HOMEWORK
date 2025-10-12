CXX := g++
CXXFLAGS := -std=c++11 -O2 -Wall -Wextra
TARGET := hacker-rm
SRCS := hacker-rm.cpp
OBJS := $(SRCS:.cpp=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	-rm -f $(OBJS) $(TARGET)
