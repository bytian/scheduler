#ifndef DYNAMPRO_H
#define DYNAMPRO_H

#include "scheduler.h"

#include <vector>
#include <set>

class Simulator;

class DynamPro : public Scheduler
{
private:
    // growth rate of tree 
    static const double ALPHA;

private:
    // the transactions waiting on a write lock
    // on each object
    std::vector<std::vector<int> > exclTrans;

    // the transactions waiting on a read lock
    // on each object
    std::vector<std::vector<int> > inclTrans;

    // Number of dependencies of a transaction
    std::vector<int> TNumDep;

    // Number of dependencies of an object
    std::vector<int> ONumDep; 
private:
    /**********************************************
     * Expected running time for r transactions 
     * to finish (Expectation of the max of r iid
     * random variables).
     **********************************************/
    double f(int r);


public:
    DynamPro();
    DynamPro(Simulator* sim);
    void init();
    bool acquire(int tid, int oid, bool excl);
    void release(int tid, int oid);
    const std::set<int> assign(int oid);
    int getTime();
}

#endif
