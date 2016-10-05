#include "fifo.h"

#include <vector>
#include <deque>
#include <set>

FIFO::FIFO(Simulator* sim) : sim(sim)
{
    exclTrans.resize(sim->getTotalObj());
    inclTrans.resize(sim->getTotalObj());
    exclTime.resize(sim->getTotalObj());
}

bool FIFO::acquire(int tid, int oid, bool excl)
{
    int status = sim->getObj(oid).getStatus();
    if (status == Object::FREE)
    {
        std::set<int> trans;
        trans.insert(tid);
        sim->getObj(oid).addOwner(trans, excl);
        return true;
    }

    if (excl)
    {
        exclTrans[oid].std::push_back(tid);
        exclTime[oid].std::push_back(sim->getTime());
    }
    else
    {
        incluTrans[oid].insert(tid);
        if (inclTime < 0)
            inclTime = sim->getTime();
    }

    return false;
}

void FIFO::release(int tid, int oid)
{
    sim->getObj(oid).releaseBy(tid);
}

void const std::set<int>& assign(int oid)
{
    std::set<int>& assigned;

    if (inclTime < 0 || inclTime > exclTime.front())
    {
        int trans = exclTrans[oid].front();
        sim->getTrans(trans).grantLock();

        assigned.insert(trans);
        exclTrans[oid].pop_front();
        exclTime[oid].pop_front();

        sim->getObj(oid).addOwner(assigned, true);
    }
    else
    {
        assigned = inclTrans[oid];
        for (auto itr = assigned.begin(); itr != end(); ++itr)
        {
            sim->getTrans(*itr).grantLock();
        }

        inclTime = -1;
        incluTrans[oid].clear();

        sim->getObj(oid).addOwner(assigned, false);
    }
    return assigned;
}

int FIFO::getTime()
{
    return sim->getTime();
}


