//
// Created by Jamie Wu on 10/14/16.
//

#include "AgeSumScheduler.h"

#include "scheduler.h"
#include "object.h"
#include "simulator.h"
#include "transaction.h"
#include <algorithm>

#include <vector>
#include <set>
#include <iostream>

AgeSumScheduler::AgeSumScheduler() : Scheduler() {}

AgeSumScheduler::AgeSumScheduler(Simulator* sim) : sim(sim) {}

void AgeSumScheduler::init() {
    exclTrans.resize(sim->getTotalObj());
    inclTrans.resize(sim->getTotalObj());
    size_age_O.resize(sim->getTotalObj());
    size_age_T.resize(sim->getTotalTrans());
    for(int i = 0; i < size_age_O.size(); i++){
        size_age_O[i].Tx_id = i;
        size_age_O[i].size = 0;
        size_age_O[i].startTimeSum = 0;
    }
    for(int i = 0; i < size_age_T.size(); i++){
        size_age_T[i].Tx_id = i;
        size_age_T[i].size = 0;
        size_age_T[i].startTimeSum = 0;
    }
}

bool AgeSumScheduler::acquire(int tid, int oid, bool excl)
{
    int status = sim->getObj(oid).getStatus();

    if (status == Object::FREE) {

        updateT(tid, size_age_O[oid].size, size_age_O[oid].startTimeSum);

        std::set<int> trans;
        trans.insert(tid);
        sim->getObj(oid).addOwner(trans, excl);
        return true;
    }

    if (excl) {
        exclTrans[oid].push_back(&size_age_T[tid]);
    }
    else {
        inclTrans[oid].push_back(&size_age_T[tid]);
    }
    updateO(oid, 1 + size_age_T[tid].size, size_age_T[tid].startTimeSum + sim->getTrans(tid).getStartTime());

    return false;
}

void AgeSumScheduler::release(int tid, int oid)
{
    sim->getObj(oid).releaseBy(tid);
    updateT(tid, -size_age_O[oid].size, -size_age_O[oid].startTimeSum);

    if (sim->getObj(oid).getStatus() == Object::FREE) {
        if (!exclTrans[oid].empty() || !inclTrans[oid].empty()) {
            sim->addToAssign(oid);
        }
    }
}

const std::set<int> AgeSumScheduler::assign(int oid)
{
    std::set<int> assigned;
    if(exclTrans[oid].size() == 0 and inclTrans[oid].size() == 0)
        return  assigned;

    sort(exclTrans[oid].begin(), exclTrans[oid].end(), myComparater);
    sort(inclTrans[oid].begin(), inclTrans[oid].end(), myComparater);

    if (inclTrans[oid].size() == 0 or (exclTrans[oid].size() > 0 and exclTrans[oid][0]->startTimeSum < inclTrans[oid][0]->startTimeSum)) {  // assign lock to write
        int trans = (*exclTrans[oid].begin())->Tx_id;
        updateO(oid, -(*exclTrans[oid].begin())->size - 1, -(*exclTrans[oid].begin())->startTimeSum - sim->getTrans(trans).getStartTime());

        sim->getTrans(trans).grantLock();

        updateT(trans, size_age_O[oid].size, size_age_O[oid].startTimeSum); // grant lock BEFORE this update ! ! !

        assigned.insert(trans);
        exclTrans[oid].erase(exclTrans[oid].begin());

        sim->getObj(oid).addOwner(assigned, true);

    } else {  // assign lock to reads

        int inclSize = 0;
        double inclAge = 0.0;
        for (auto itr = inclTrans[oid].begin(); itr != inclTrans[oid].end(); ++itr) {
            int tx = (*itr)->Tx_id;
            inclSize += (*itr)->size + 1;
            inclAge += (*itr)->startTimeSum + sim->getTrans(tx).getStartTime();
        }
        updateO(oid, -inclSize, inclAge);

        for (auto itr = inclTrans[oid].begin(); itr != inclTrans[oid].end(); ++itr) {
            int trans = (*itr)->Tx_id;
            sim->getTrans(trans).grantLock();
            updateT(trans, size_age_O[oid].size, size_age_O[oid].startTimeSum); // grant lock BEFORE this update ! ! !
            assigned.insert(trans);
        }

        inclTrans[oid].clear();

        sim->getObj(oid).addOwner(assigned, false);

    }

    return assigned;
}


int AgeSumScheduler::getTime() { return sim->getTime(); }


void AgeSumScheduler::updateO(int oid, int delta_size, double delta_start_time) {
    size_age_O[oid].size += delta_size;
    size_age_O[oid].startTimeSum += delta_start_time;
    for (auto itr = sim->getObj(oid).getOwner().begin(); itr != sim->getObj(oid).getOwner().end(); ++itr) {
        updateT(*itr, delta_size, delta_start_time);
    }
}

void AgeSumScheduler::updateT(int tid, int delta_size, double delta_start_time) {
    size_age_T[tid].size += delta_size;
    size_age_T[tid].startTimeSum += delta_start_time;
    int oid = sim->getTrans(tid).getBlockBy();
    if (oid > 0) {
        updateO(oid, delta_size, delta_start_time);
    }
}

