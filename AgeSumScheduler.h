//
// Created by Jamie Wu on 10/14/16.
//

#ifndef SCHEDULER_SUMAGELARGESTFIRST_H
#define SCHEDULER_SUMAGELARGESTFIRST_H

#include <vector>
#include "scheduler.h"

class Simulator;

using namespace std;

struct Tx_size_age{
    int Tx_id;
    int size;
    double startTimeSum;
};

class AgeSumScheduler : public Scheduler {
private:
    struct Comparater { // customized comparater for the priority queue
        bool operator() (Tx_size_age* Ta, Tx_size_age* Tb) {
            if (Ta->size * Ta->startTimeSum > Tb->size * Tb->startTimeSum)
                return true;
            else
                return false;
        }
    } myComparater;

    vector<Tx_size_age> size_age_O;
    vector<Tx_size_age> size_age_T;
    vector<vector<Tx_size_age*>> exclTrans;
    vector<vector<Tx_size_age*>> inclTrans;
    Simulator* sim;

private:
    void updateO(int oid, int delta_size, double delta_start_time);
    void updateT(int tid, int delta_size, double delta_start_time);

public:
    AgeSumScheduler();
    AgeSumScheduler(Simulator* sim);
    void init();
    bool acquire(int tid, int oid, bool excl);
    void release(int tid, int oid);
    const std::set<int> assign(int oid);
    int getTime();
};

#endif //SCHEDULER_SUMAGELARGESTFIRST_H
