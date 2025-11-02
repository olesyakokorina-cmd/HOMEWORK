CC = g++
CFLAGS = -std=c++11 -O2 -pthread
TARGET = array-summary
SRC = array-summary.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

