.PHONY: all clean

CXX=g++ -std=c++11

all : oldest fifo random dtsize dp nosplit


oldest: scheduler.cpp NoSplitReadsScheduler.cpp fifo.cpp dp.cpp randomScheduler.cpp oldestFirst.cpp transaction.cpp simulator.cpp run.cpp object.cpp decoupledTSize.cpp
	$(CXX) -o $@ $^ -DSIM_SCHEDULER=OldestFirst

fifo: scheduler.cpp NoSplitReadsScheduler.cpp fifo.cpp dp.cpp randomScheduler.cpp oldestFirst.cpp transaction.cpp simulator.cpp run.cpp object.cpp decoupledTSize.cpp
	$(CXX) -o $@ $^ -DSIM_SCHEDULER=FIFO

random: scheduler.cpp NoSplitReadsScheduler.cpp fifo.cpp dp.cpp randomScheduler.cpp oldestFirst.cpp transaction.cpp simulator.cpp run.cpp object.cpp decoupledTSize.cpp
	$(CXX) -o $@ $^ -DSIM_SCHEDULER=RandomScheduler

dtsize: scheduler.cpp NoSplitReadsScheduler.cpp fifo.cpp dp.cpp randomScheduler.cpp oldestFirst.cpp transaction.cpp simulator.cpp run.cpp object.cpp decoupledTSize.cpp
	$(CXX) -o $@ $^ -DSIM_SCHEDULER=DecoupledTSize

dp: scheduler.cpp NoSplitReadsScheduler.cpp fifo.cpp dp.cpp randomScheduler.cpp oldestFirst.cpp transaction.cpp simulator.cpp run.cpp object.cpp decoupledTSize.cpp
	$(CXX) -o $@ $^ -DSIM_SCHEDULER=DP

nosplit: scheduler.cpp NoSplitReadsScheduler.cpp fifo.cpp dp.cpp randomScheduler.cpp oldestFirst.cpp transaction.cpp simulator.cpp run.cpp object.cpp decoupledTSize.cpp


clean:
	rm -f gen oldest fifo random dtsize dp nosplit

