CXX = g++
CXXFLAGS = -std=c++11

all: signal-echo

signal-echo: signal_echo.o
	$(CXX) $(CXXFLAGS) -o signal-echo signal_echo.o

signal_echo.o: signal_echo.cpp
	$(CXX) $(CXXFLAGS) -c signal_echo.cpp

clean:
	rm -f *.o signal-echo

