.PHONY: all clean

CXX=g++ -std=c++11

all : oldest fifo random dtsize dp nosplit


oldest: scheduler.cpp oldestFirst.cpp transaction.cpp simulator.cpp run.cpp object.cpp 
	$(CXX) -o $@ $^ -DOLDEST

fifo: scheduler.cpp fifo.cpp transaction.cpp simulator.cpp run.cpp object.cpp 
	$(CXX) -o $@ $^ -DFifo

random: scheduler.cpp randomScheduler.cpp transaction.cpp simulator.cpp run.cpp object.cpp 
	$(CXX) -o $@ $^ -DRANDOM

dtsize: scheduler.cpp transaction.cpp simulator.cpp run.cpp object.cpp decoupledTSize.cpp
	$(CXX) -o $@ $^ -DDTSIZE

dp: scheduler.cpp dp.cpp transaction.cpp simulator.cpp run.cpp object.cpp 
	$(CXX) -o $@ $^ -DDp

nosplit: scheduler.cpp NoSplitReadsScheduler.cpp transaction.cpp simulator.cpp run.cpp object.cpp 
	$(CXX) -o $@ $^ -DNOSPLIT

clean:
	rm -f gen oldest fifo random dtsize dp nosplit

