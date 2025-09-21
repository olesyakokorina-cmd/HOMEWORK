CXX = g++
CXXFLAGS = -std=c++11 -Wall

TARGET = complex_sort
OBJS = main.o Complex.o sort_complex.o

all: $(TARGET)

$(TARGET): $(OBJS)
  $(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

main.o: main.cpp Complex.h sort_complex.h
  $(CXX) $(CXXFLAGS) -c main.cpp

Complex.o: Complex.cpp Complex.h
  $(CXX) $(CXXFLAGS) -c Complex.cpp

sort_complex.o: sort_complex.cpp sort_complex.h Complex.h
  $(CXX) $(CXXFLAGS) -c sort_complex.cpp

clean:
	rm -f $(OBJS) $(TARGET)

