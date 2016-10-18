.PHONY: all clean

CXX=g++ -std=c++11

all : gen oldest fifo random dtsize dp nosplit constchunk agesum swap

gen: gen.cpp
	$(CXX) -o $@ $^ -O3

oldest: scheduler.cpp oldestFirst.cpp transaction.cpp simulator.cpp run.cpp object.cpp 
	$(CXX) -o $@ $^ -DOLDEST -O3

fifo: scheduler.cpp fifo.cpp transaction.cpp simulator.cpp run.cpp object.cpp 
	$(CXX) -o $@ $^ -DFifo -O3

random: scheduler.cpp randomScheduler.cpp transaction.cpp simulator.cpp run.cpp object.cpp 
	$(CXX) -o $@ $^ -DRANDOM -O3

dtsize: scheduler.cpp transaction.cpp simulator.cpp run.cpp object.cpp decoupledTSize.cpp
	$(CXX) -o $@ $^ -DDTSIZE -O3

dp: scheduler.cpp dp.cpp transaction.cpp simulator.cpp run.cpp object.cpp 
	$(CXX) -o $@ $^ -DDp -O3

nosplit: scheduler.cpp NoSplitReadsScheduler.cpp transaction.cpp simulator.cpp run.cpp object.cpp 
	$(CXX) -o $@ $^ -DNOSPLIT -O3

constchunk: scheduler.cpp ConstantChunkScheduler.cpp transaction.cpp simulator.cpp run.cpp object.cpp 
	$(CXX) -o $@ $^ -DCONSTCHUNK -O3

agesum: scheduler.cpp AgeSumScheduler.cpp transaction.cpp simulator.cpp run.cpp object.cpp 
	$(CXX) -o $@ $^ -DAGESUM -O3

swap: scheduler.cpp swapping.cpp transaction.cpp simulator.cpp run.cpp object.cpp 
	$(CXX) -o $@ $^ -DSWAP -O3

clean:
	rm -f gen oldest fifo random dtsize dp nosplit constchunk agesum

