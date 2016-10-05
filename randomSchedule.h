//
// Created by Jamie Wu on 10/5/16.
//

#ifndef SCHEDULER_RANDOMSCHEDULE_H
#define SCHEDULER_RANDOMSCHEDULE_H


#include <vector>
#include <list>
#include <set>

#include "scheduler.h"

class Simulator;

class RandomScheduler : public Scheduler
{
private:
    std::vector<std::vector<int> > exclTrans;
    std::vector<std::set<int> > inclTrans;

    Simulator* sim;
public:
    RandomScheduler();
    RandomScheduler(Simulator* sim);
    void init();
    bool acquire(int tid, int oid, bool excl);
    void release(int tid, int oid);
    const std::set<int> assign(int oid);
    int getTime();
};


#endif //SCHEDULER_RANDOMSCHEDULE_H

