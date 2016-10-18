#ifndef SWAPPING_H
#define SWAPPING_H

#include <vector>
#include "scheduler.h"

class Simulator;

class Swapping : public Scheduler
{
private:
    std::vector<std::vector<int> > schedule;
    std::vector<std::vector<int> > inclTrans;
    std::vector<int> releaseTime;
    std::vector<int> finishTime;
    Simulator* sim;

private:
    void updateO(int oid, int& latency);
    void updateT(int tid, int delta, int& latency);
    void swapFrom(int oid);
    bool swapO(int oid);
    bool swap(int oid, int x);

public:
    Swapping();
    Swapping(Simulator* sim);
    void init();
    bool acquire(int tid, int oid, bool excl);
    void release(int tid, int oid);
    const std::set<int> assign(int oid);
    int getTime();
};


#endif // SWAPPING_H
