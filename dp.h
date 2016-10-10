#ifndef DP_H
#define DP_H

#include <vector>
#include <cmath>
#include "scheduler.h"

class Simulator;

class DP : public Scheduler
{
private:
    class Cmp
    {
    private:
        DP* dp;
    public:
        Cmp(DP* dp) : dp(dp) {}
        bool operator()(int t1, int t2) { return dp->sizeT[t1] > dp->sizeT[t2]; }
    };
private:
    double f(int r); 
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
    int run_dp(int oid);

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
