//
// Created by Jamie Wu on 10/5/16.
//

#include "OldestFirst.h"
#include "object.h"
#include "simulator.h"
#include "transaction.h"


OldestFirstScheduler::OldestFirstScheduler() : Scheduler() {}

OldestFirstScheduler::OldestFirstScheduler(Simulator* sim) : Scheduler(sim) {}

void OldestFirstScheduler::init()
{
    exclTrans.resize(sim->getTotalObj());
    inclTrans.resize(sim->getTotalObj());
    //exclTime.resize(sim->getTotalObj());
    minInclStartTime.resize(sim->getTotalObj(), -1);
}

bool OldestFirstScheduler::acquire(int tid, int oid, bool excl)
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

void OldestFirstScheduler::release(int tid, int oid)
{
    sim->getObj(oid).releaseBy(tid);
}

const std::set<int> OldestFirstScheduler::assign(int oid)
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

int OldestFirstScheduler::getTime() { return sim->getTime(); }
