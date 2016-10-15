.PHONY: all clean

CXX=g++ -std=c++11

all : oldest fifo random dtsize dp nosplit constchunk agesum


oldest: scheduler.cpp oldestFirst.cpp transaction.cpp simulator.cpp run.cpp object.cpp 
	$(CXX) -o $@ $^ -DOLDEST -O2

fifo: scheduler.cpp fifo.cpp transaction.cpp simulator.cpp run.cpp object.cpp 
	$(CXX) -o $@ $^ -DFifo -O2

random: scheduler.cpp randomScheduler.cpp transaction.cpp simulator.cpp run.cpp object.cpp 
	$(CXX) -o $@ $^ -DRANDOM -O2

dtsize: scheduler.cpp transaction.cpp simulator.cpp run.cpp object.cpp decoupledTSize.cpp
	$(CXX) -o $@ $^ -DDTSIZE -O2

dp: scheduler.cpp dp.cpp transaction.cpp simulator.cpp run.cpp object.cpp 
	$(CXX) -o $@ $^ -DDp -O2

nosplit: scheduler.cpp NoSplitReadsScheduler.cpp transaction.cpp simulator.cpp run.cpp object.cpp 
	$(CXX) -o $@ $^ -DNOSPLIT -O2

constchunk: scheduler.cpp ConstantChunkScheduler.cpp transaction.cpp simulator.cpp run.cpp object.cpp 
	$(CXX) -o $@ $^ -DCONSTCHUNK -O2

agesum: scheduler.cpp AgeSumScheduler.cpp transaction.cpp simulator.cpp run.cpp object.cpp 
	$(CXX) -o $@ $^ -DAGESUM -O2

clean:
	rm -f gen oldest fifo random dtsize dp nosplit constchunk agesum

