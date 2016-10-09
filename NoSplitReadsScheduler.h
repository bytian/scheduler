//
// Created by Jamie Wu on 10/9/16.
//

#ifndef SCHEDULER_NOSPLITREADSSCHEDULER_H
#define SCHEDULER_NOSPLITREADSSCHEDULER_H

#include <vector>
#include <queue>
#include "scheduler.h"
#include "transaction.h"
#include <utility>
#include <math.h>

using namespace std;
class Simulator;


class NoSplitReadsScheduler : public Scheduler
{
private:
    struct Comparater { // customized comparater for the priority queue
        bool operator() (pair<int, int>* Ta, pair<int, int>* Tb)
        {
            if (Ta->second > Tb->second)
                return true;
            else
                return false;
        }
    } myComparater;
    vector<int> sizeO;
    vector< pair<int, int> > sizeT;
    vector< vector< pair<int, int>*> > exclTrans;
    vector<std::vector<int> > inclTrans;
    Simulator* sim;



private:
    double f(int r) { return log(1.0 + r) / log(2.);}
    void updateO(int oid, int delta);
    void updateT(int tid, int delta);

public:
    NoSplitReadsScheduler();
    NoSplitReadsScheduler(Simulator* sim);
    void init();
    bool acquire(int tid, int oid, bool excl);
    void release(int tid, int oid);
    const std::set<int> assign(int oid);
    int getTime();
};


#endif //SCHEDULER_NOSPLITREADSSCHEDULER_H
