//
// Created by Jamie Wu on 10/11/16.
//

#ifndef SCHEDULER_CONSTANTCHUNKSCHEDULER_H
#define SCHEDULER_CONSTANTCHUNKSCHEDULER_H

#include <vector>
#include <queue>
#include "scheduler.h"
#include "transaction.h"
#include <utility>
#include <math.h>

using namespace std;
class Simulator;


class ConstantChunkScheduler : public Scheduler
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
    vector< vector< pair<int, int>* > > exclTrans;
    vector< vector< pair<int, int>* > > inclTrans;
    Simulator* sim;
    int chunkSize;
private:
    double f(int r) { return log(1.0 + r) / log(2.);}
    void updateO(int oid, int delta);
    void updateT(int tid, int delta);

public:
    ConstantChunkScheduler();
    ConstantChunkScheduler(Simulator* sim);
    ConstantChunkScheduler(Simulator* sim, int chunkSize);
    void init();
    bool acquire(int tid, int oid, bool excl);
    void release(int tid, int oid);
    const std::set<int> assign(int oid);
    int getTime();
};

#endif //SCHEDULER_CONSTANTCHUNKSCHEDULER_H
