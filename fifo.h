#ifndef FIFO_H
#define FIFO_H

#include <vector>
#include <deque>
#include <set>

//#include "object.h"
//#include "transaction.h"
#include "scheduler.h"
//#include "simulator.h"

class Simulator;

class FIFO : public Scheduler
{
private:
    std::vector<std::deque<int> > exclTrans;
    std::vector<std::set<int> > inclTrans;
    std::vector<std::deque<int> > exclTime;
    std::vector<int> inclTime;
    Simulator* sim;
public:
    FIFO();
    FIFO(Simulator* sim);
    void init();
    bool acquire(int tid, int oid, bool excl);
    void release(int tid, int oid);
    const std::set<int> assign(int oid);
    int getTime();
};

#endif
