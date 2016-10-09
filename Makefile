.PHONY: all clean

CXX=g++ -std=c++11

all : oldest fifo random dtsize


oldest: scheduler.cpp fifo.cpp randomScheduler.cpp oldestFirst.cpp transaction.cpp simulator.cpp run.cpp object.cpp decoupledTSize.cpp
	$(CXX) -o $@ $^ -DSIM_SCHEDULER=OldestFirst

fifo: scheduler.cpp fifo.cpp randomScheduler.cpp oldestFirst.cpp transaction.cpp simulator.cpp run.cpp object.cpp decoupledTSize.cpp
	$(CXX) -o $@ $^ -DSIM_SCHEDULER=FIFO

random: scheduler.cpp fifo.cpp randomScheduler.cpp oldestFirst.cpp transaction.cpp simulator.cpp run.cpp object.cpp decoupledTSize.cpp
	$(CXX) -o $@ $^ -DSIM_SCHEDULER=RandomScheduler

dtsize: scheduler.cpp fifo.cpp randomScheduler.cpp oldestFirst.cpp transaction.cpp simulator.cpp run.cpp object.cpp decoupledTSize.cpp
	$(CXX) -o $@ $^ -DSIM_SCHEDULER=DecoupledTSize



clean:
	rm -f gen oldest fifo random dtsize

