#include "fifo_re.h"
#include "scheduler.h"
#include "object.h"
#include "simulator.h"
#include "transaction.h"

#include <vector>
#include <deque>
#include <set>
#include <cstdlib>
#include <iostream>

FIFO_RE::FIFO_RE() : Scheduler() {}

FIFO_RE::FIFO_RE(Simulator* sim) : sim(sim) {}

void FIFO_RE::init()
{
    queue.resize(sim->getTotalObj());
    inclTrans.resize(sim->getTotalObj());
}

bool FIFO_RE::acquire(int tid, int oid, bool excl)
{
    int status = sim->getObj(oid).getStatus();

    if (status == Object::FREE && queue[oid].empty())
    {
        std::set<int> trans;
        trans.insert(tid);
        sim->getObj(oid).addOwner(trans, excl);
        return true;
    }

    if (excl)
    {
        queue[oid].push_back(tid);
    }
    else
    {
        if (inclTrans[oid].empty())
        {
            queue[oid].push_back(-1);
        }
        inclTrans[oid].push_back(tid);
    }

    return false;
}

void FIFO_RE::release(int tid, int oid)
{
    sim->getObj(oid).releaseBy(tid);

    if (sim->getObj(oid).getStatus() == Object::FREE)
    {
        if (!queue[oid].empty())
        {
            sim->addToAssign(oid);
        }
    }
}

const std::set<int> FIFO_RE::assign(int oid)
{
    std::set<int> assigned;

    int trans = queue[oid].front();

    if (trans != -1)
    {
        sim->getTrans(trans).grantLock();
        assigned.insert(trans);
        sim->getObj(oid).addOwner(assigned, true);
    }
    else
    {
        for (auto itr = inclTrans[oid].begin(); itr != inclTrans[oid].end(); ++itr)
        {
            assigned.insert(*itr);
            sim->getTrans(*itr).grantLock();
        }
        inclTrans[oid].clear();
        sim->getObj(oid).addOwner(assigned, false);
    }

    queue[oid].pop_front();

    return assigned;
}

int FIFO_RE::getTime() { return sim->getTime(); }
