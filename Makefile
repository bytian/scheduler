.PHONY: all clean

CXX=g++ -std=c++11

all: run

run: scheduler.cpp fifo.cpp transaction.cpp simulator.cpp run.cpp object.cpp
	$(CXX) -o $@ $^

clean:
	rm -f run gen
