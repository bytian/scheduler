//
// Created by Jamie Wu on 10/5/16.
//

#ifndef SCHEDULER_OLDESTFIRST_H
#define SCHEDULER_OLDESTFIRST_H

#include <vector>
#include <deque>
#include <set>
#include <queue>

#include "transaction.h"
#include "scheduler.h"

class Simulator;

struct Comparater { // customized comparater for the priority queue
    bool operator() (Transaction* Ta, Transaction* Tb)
    {
        if (Ta->getStartTime() > Tb->getStartTime())
            return true;
        else
            return false;
    }
};

class OldestFirst : public Scheduler {
private:
    std::vector< std::priority_queue< Transaction*, std::vector<Transaction*>, Comparater> > exclTrans;
    std::vector<std::set<int> > inclTrans;
    std::vector<int> minInclStartTime;
    Simulator* sim;
public:
    OldestFirst();
    OldestFirst(Simulator* sim);
    void init();
    bool acquire(int tid, int oid, bool excl);
    void release(int tid, int oid);
    const std::set<int> assign(int oid);
    int getTime();
};


#endif //SCHEDULER_OLDESTFIRST_H
