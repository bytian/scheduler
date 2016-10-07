.PHONY: all clean

CXX=g++ -std=c++11

all: run

run: scheduler.cpp oldestFirst.cpp transaction.cpp simulator.cpp run.cpp object.cpp
	$(CXX) -o $@ $^

clean:
	rm -f run gen
