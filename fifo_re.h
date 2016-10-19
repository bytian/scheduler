#ifndef FIFO_RE_H
#define FIFO_RE_H

#include <deque>
#include <vector>
#include <set>

#include "scheduler.h"

class Simulator;

class FIFO_RE : public Scheduler
{
private:
    std::vector<std::deque<int> > queue;
    std::vector<std::vector<int> > inclTrans;
    Simulator* sim;

public:
    FIFO_RE();
    FIFO_RE(Simulator* sim);
    void init();
    bool acquire(int tid, int oid, bool excl);
    void release(int tid, int oid);
    const std::set<int> assign(int oid);
    int getTime();
};

#endif // FIFO_RE_H
