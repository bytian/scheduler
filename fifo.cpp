#include "fifo.h"
#include "scheduler.h"
#include "object.h"
#include "transaction.h"
#include "simulator.h"

#include <vector>
#include <deque>
#include <set>
#include <cstdlib>
#include <iostream>

FIFO::FIFO() : Scheduler() {}

FIFO::FIFO(Simulator* sim) : sim(sim)
{
    exclTrans.resize(sim->getTotalObj());
    inclTrans.resize(sim->getTotalObj());
    exclTime.resize(sim->getTotalObj());
    inclTime.resize(sim->getTotalObj(), -1);
}

bool FIFO::acquire(int tid, int oid, bool excl)
{
    std::cerr << "\t\t\t-1" << '\t' << oid  << '\t' << sim->getTotalObj() << std::endl;
    int status = sim->getObj(oid).getStatus();
    std::cerr << "\t\t\t000" << std::endl;
    if (status == Object::FREE)
    {
        std::set<int> trans;
        trans.insert(tid);
        std::cerr << "\t\t\t1" << std::endl;
        sim->getObj(oid).addOwner(trans, excl);
        std::cerr << "\t\t\t2" << std::endl;
        return true;
    }

    if (excl)
    {
        exclTrans[oid].push_back(tid);
        exclTime[oid].push_back(sim->getTime());
    }
    else
    {
        inclTrans[oid].insert(tid);
        if (inclTime[oid] < 0)
            inclTime[oid] = sim->getTime();
    }

    return false;
}

void FIFO::release(int tid, int oid)
{
    sim->getObj(oid).releaseBy(tid);
}

const std::set<int> FIFO::assign(int oid)
{
    std::set<int> assigned;

    if (inclTime[oid] < 0 || inclTime[oid] > exclTime[oid].front())
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
        for (auto itr = assigned.begin(); itr != assigned.end(); ++itr)
        {
            sim->getTrans(*itr).grantLock();
        }

        inclTime[oid] = -1;
        inclTrans[oid].clear();

        sim->getObj(oid).addOwner(assigned, false);
    }
    return assigned;
}

int FIFO::getTime()
{
    return sim->getTime();
}


