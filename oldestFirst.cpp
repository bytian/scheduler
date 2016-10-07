//
// Created by Jamie Wu on 10/5/16.
//

#include "oldestFirst.h"
#include "object.h"
#include "simulator.h"
#include "transaction.h"


OldestFirst::OldestFirst() : Scheduler() {}

OldestFirst::OldestFirst(Simulator* sim) : sim(sim) {}

void OldestFirst::init()
{
    exclTrans.resize(sim->getTotalObj());
    inclTrans.resize(sim->getTotalObj());
    //exclTime.resize(sim->getTotalObj());
    minInclStartTime.resize(sim->getTotalObj(), -1);
}

bool OldestFirst::acquire(int tid, int oid, bool excl)
{
    int status = sim->getObj(oid).getStatus();
    if (status == Object::FREE) {
        std::set<int> trans;
        trans.insert(tid);
        sim->getObj(oid).addOwner(trans, excl);
        return true;
    }

    if (excl) {
        exclTrans[oid].push(&sim->getTrans(tid));
    }
    else
    {
        inclTrans[oid].insert(tid);
        int startTime = sim->getTrans(oid).getStartTime();
        if (minInclStartTime[oid] < 0)
            minInclStartTime[oid] = sim->getTime();
        else if (minInclStartTime[oid] > startTime)
            minInclStartTime[oid] = startTime;
    }

    return false;
}

void OldestFirst::release(int tid, int oid)
{
    sim->getObj(oid).releaseBy(tid);
    if (sim->getObj(oid).getStatus() == Object::FREE)
    {
        if (!exclTrans[oid].empty() || !inclTrans[oid].empty())
        {
            sim->addToAssign(oid);
        }
    }
}

const std::set<int> OldestFirst::assign(int oid)
{
    std::set<int> assigned;

    if (minInclStartTime[oid] < 0 || minInclStartTime[oid] > exclTrans[oid].top()->getStartTime() ) { // assign the write lock
        int trans = exclTrans[oid].top()->getID();
        sim->getTrans(trans).grantLock();

        assigned.insert(trans);
        exclTrans[oid].pop();
        //exclTime[oid].pop_front();

        sim->getObj(oid).addOwner(assigned, true);
    }
    else // assign the read lock
    {
        assigned = inclTrans[oid];
        for (auto itr = assigned.begin(); itr != assigned.end(); ++itr) {
            sim->getTrans(*itr).grantLock();
        }

        minInclStartTime[oid] = -1;
        inclTrans[oid].clear();

        sim->getObj(oid).addOwner(assigned, false);
    }
    return assigned;
}

int OldestFirst::getTime() { return sim->getTime(); }
