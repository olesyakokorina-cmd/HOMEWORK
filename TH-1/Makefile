CXX = g++
CXXFLAGS = -O2 -Wall
LDFLAGS = -lpthread
TARGET = array-summary
SRC = array-summary.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

clean:
	rm -f $(TARGET)

