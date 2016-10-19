#ifndef LARGESTDEPENDENCY_H
#define LARGESTDEPENDENCY_H

#include <vector>
#include "scheduler.h"

class simulator;

class LargestDependency : public Scheduler
{
private:
    std::vector<std::vector<int> > exclTrans;
    std::vector<std::vector<int> > inclTrans;
    Simulator* sim;

private:
    int getDepSize(std::vector<int>::iterator begin, std::vector<int>::iterator end);

public:
    LargestDependency();
    LargestDependency(Simulator* sim);
    void init();
    bool acquire(int tid, int oid, bool excl);
    void release(int tid, int oid);
    const std::set<int> assign(int oid);
    int getTime();
};

#endif // LARGESTDEPENDENCY_H
