.PHONY: all clean

CXX=g++ -std=c++11

all : oldest fifo random


oldest: scheduler.cpp fifo.cpp randomScheduler.cpp oldestFirst.cpp transaction.cpp simulator.cpp run.cpp object.cpp 
	$(CXX) -o $@ $^ -DSIM_SCHEDULER=OldestFirst

fifo: scheduler.cpp fifo.cpp randomScheduler.cpp oldestFirst.cpp transaction.cpp simulator.cpp run.cpp object.cpp 
	$(CXX) -o $@ $^ -DSIM_SCHEDULER=FIFO

random: scheduler.cpp fifo.cpp randomScheduler.cpp oldestFirst.cpp transaction.cpp simulator.cpp run.cpp object.cpp 
	$(CXX) -o $@ $^ -DSIM_SCHEDULER=RandomScheduler



clean:
	rm -f gen oldest fifo random

