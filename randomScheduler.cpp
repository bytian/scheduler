//
// Created by Jamie Wu on 10/5/16.
//

#include "randomScheduler.h"
#include "scheduler.h"
#include "object.h"
#include "simulator.h"
#include "transaction.h"
#include <cstdlib>

#include <iostream>

RandomScheduler::RandomScheduler() : Scheduler() {}

RandomScheduler::RandomScheduler(Simulator* sim) : sim(sim) {}

void RandomScheduler::init() {
    exclTrans.resize(sim->getTotalObj());
    inclTrans.resize(sim->getTotalObj());
}

bool RandomScheduler::acquire(int tid, int oid, bool excl)
{
    int status = sim->getObj(oid).getStatus();
    if (status == Object::FREE) {
        std::set<int> trans;
        trans.insert(tid);
        sim->getObj(oid).addOwner(trans, excl);
        return true;
    }

    if (excl) {
        exclTrans[oid].push_back(tid);
    }
    else {
        inclTrans[oid].insert(tid);
    }

    return false;
}

void RandomScheduler::release(int tid, int oid)
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

const std::set<int> RandomScheduler::assign(int oid)
{
    std::set<int> assigned;

    double r = ((double) rand() / (RAND_MAX));

    if((r < 0.5 && !inclTrans[oid].empty()) || exclTrans[oid].empty()){ // assign read lock
        assigned = inclTrans[oid];
        for (auto itr = assigned.begin(); itr != assigned.end(); ++itr) {
            sim->getTrans(*itr).grantLock();
        }
        inclTrans[oid].clear();
        sim->getObj(oid).addOwner(assigned, false);

    } else { // assign write lock
        int which_write = rand() % exclTrans[oid].size(); // generate a random integer from 0 to exclTrans.size()
        int trans = exclTrans[oid][which_write];
        sim->getTrans(trans).grantLock();

        assigned.insert(trans);
        exclTrans[oid].erase(exclTrans[oid].begin() + which_write);

        sim->getObj(oid).addOwner(assigned, true);
    }

    return assigned;
}

int RandomScheduler::getTime() { return sim->getTime(); }

