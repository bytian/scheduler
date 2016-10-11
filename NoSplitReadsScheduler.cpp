//
// Created by Jamie Wu on 10/9/16.
//

#include "NoSplitReadsScheduler.h"
#include "object.h"
#include "simulator.h"
#include <iostream>
#include <algorithm>

using namespace std;

NoSplitReadsScheduler::NoSplitReadsScheduler() : Scheduler() {}

NoSplitReadsScheduler::NoSplitReadsScheduler(Simulator* sim) : sim(sim) {}

void NoSplitReadsScheduler::init() {

    exclTrans.resize(sim->getTotalObj());
    inclTrans.resize(sim->getTotalObj());
    sizeO.resize(sim->getTotalObj(), 0);
    sizeT.resize(sim->getTotalTrans(), make_pair(0, 0));
    for(int i = 0; i < sizeT.size(); i ++){
        sizeT[i].first = i;
        sizeT[i].second = 0;
    }
}

bool NoSplitReadsScheduler::acquire(int tid, int oid, bool excl) {

    int status = sim->getObj(oid).getStatus();

    if (status == Object::FREE) {
        updateT(tid, sizeO[oid]);
        std::set<int> trans;
        trans.insert(tid);
        sim->getObj(oid).addOwner(trans, excl);
        return true;
    }

    if (excl) {
        exclTrans[oid].push_back(&sizeT[tid]);
        updateO(oid, 1 + sizeT[tid].second);
    }
    else {
        inclTrans[oid].push_back(tid);
        updateO(oid, 1 + sizeT[tid].second);
    }

    return false;
}

void NoSplitReadsScheduler::release(int tid, int oid) {

    sim->getObj(oid).releaseBy(tid);
    updateT(tid, -sizeO[oid]);

    if (sim->getObj(oid).getStatus() == Object::FREE) {
        if (!exclTrans[oid].empty() || !inclTrans[oid].empty()) {
            sim->addToAssign(oid);
        }
    }
}

const std::set<int> NoSplitReadsScheduler::assign(int oid) {

    std::set<int> assigned;

    std::sort(exclTrans[oid].begin(), exclTrans[oid].end(), myComparater);

    int inclSize = 0;
    for (auto itr = inclTrans[oid].begin(); itr != inclTrans[oid].end(); ++itr) {
        inclSize += sizeT[*itr].second + 1;
    }

    int n= exclTrans[oid].size(), min_position = exclTrans[oid].size();

    double latency = n * inclSize, min_latency = n * inclSize;
    double readTime = f(inclTrans[oid].size());

    while(n > 0) {

        latency += (- inclSize + readTime * exclTrans[oid][n-1]->second);

        if (min_latency > latency) {
            min_latency = latency;
            min_position --;
        }
        n --;
    }



    if (min_position > 0) { //assign the lock to write
        int trans = exclTrans[oid][0]->first;

        updateO(oid, - exclTrans[oid][0]->second - 1);

        sim->getTrans(trans).grantLock();

        updateT(trans, sizeO[oid]); // grant lock BEFORE this update ! ! !

        assigned.insert(trans);
        exclTrans[oid].erase(exclTrans[oid].begin());

        sim->getObj(oid).addOwner(assigned, true);
    }
    else {  //assign the lock to reads
        updateO(oid, - inclSize);
        for (auto itr = inclTrans[oid].begin(); itr != inclTrans[oid].end(); ++itr)
        {
            sim->getTrans(*itr).grantLock();
            updateT(*itr, sizeO[oid]); // grant lock BEFORE this update ! ! !
            assigned.insert(*itr);
        }

        inclTrans[oid].clear();

        sim->getObj(oid).addOwner(assigned, false);
    }

    return assigned;
}

int NoSplitReadsScheduler::getTime() { return sim->getTime(); }

void NoSplitReadsScheduler::updateO(int oid, int delta) {
    sizeO[oid] += delta;
    for (auto itr = sim->getObj(oid).getOwner().begin(); itr != sim->getObj(oid).getOwner().end(); ++itr)
    {
        updateT(*itr, delta);
    }
}

void NoSplitReadsScheduler::updateT(int tid, int delta) {
    sizeT[tid].second += delta;
    int oid = sim->getTrans(tid).getBlockBy();
    if (oid > 0)
    {
        updateO(oid, delta);
    }
}
