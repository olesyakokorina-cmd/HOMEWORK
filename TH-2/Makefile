CXX = g++
CXXFLAGS = -std=c++11 -pthread

all: demo-application

demo-application: demo.o libparallel_scheduler.so
	$(CXX) -o demo-application demo.o -L. -lparallel_scheduler -Wl,-rpath,.

demo.o: demo.cpp parallel_scheduler.h
	$(CXX) $(CXXFLAGS) -c demo.cpp

libparallel_scheduler.so: parallel_scheduler.o
	$(CXX) -shared -o libparallel_scheduler.so parallel_scheduler.o -pthread

parallel_scheduler.o: parallel_scheduler.cpp parallel_scheduler.h
	$(CXX) $(CXXFLAGS) -fPIC -c parallel_scheduler.cpp

clean:
	rm -f *.o *.so demo-application

