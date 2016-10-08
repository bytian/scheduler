#ifndef DECOUPLEDTSIZE_H
#define DECOUPLEDTSIZE_H

#include <vector>
#include "scheduler.h"

class Simulator;

class DecoupledTSize : public Scheduler
{
private:
    std::vector<int> sizeO;
    std::vector<int> sizeT;
    std::vector<std::vector<int> > exclTrans;
    std::vector<std::vector<int> > inclTrans;

private:
    void updateO(int oid, int delta);
    void updateT(int tid, int delta);

public:
    DecoupledTSize();
    DecoupledTSize(Simulator* sim);
    void init();
    bool acquire(int tid, int oid, bool excl);
    void release(int tid, int oid);
    const std::set<int> assign(int oid);
    int getTime();
};

#endif // DECOUPLEDTSIZE
