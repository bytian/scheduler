#ifndef DP_H
#define DP_H

#include <vector>
#include <cmath>
#include "scheduler.h"

class Simulator;

class DP : public Scheduler
{
private:
    double f(int r) { return log(1. + r) / log(2.); }
    const static double ALPHA;

private: 
    std::vector<int> sizeO;
    std::vector<int> sizeT;
    std::vector<std::vector<int> > exclTrans;
    std::vector<std::vector<int> > inclTrans;
    Simulator* sim;

private:
    void updateO(int oid, int delta);
    void updateT(int tid, int delta);
    const std::set<int> run_dp(int oid);
    bool cmp(int, int) const;

public:
    DP();
    DP(Simulator* sim);
    void init();
    bool acquire(int tid, int oid, bool excl);
    void release(int tid, int oid);
    const std::set<int> assign(int oid);
    int getTime();
};

#endif // DP_H
