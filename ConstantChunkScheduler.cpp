//
// Created by Jamie Wu on 10/11/16.
//

#include "ConstantChunkScheduler.h"

#include "object.h"
#include "simulator.h"
#include <iostream>
#include <algorithm>
#include <numeric>

using namespace std;

ConstantChunkScheduler::ConstantChunkScheduler() : Scheduler() {}

ConstantChunkScheduler::ConstantChunkScheduler(Simulator* sim) : sim(sim) {}

ConstantChunkScheduler::ConstantChunkScheduler(Simulator* sim, int chkSize) : sim(sim) { ChunkSize = chkSize;}

void ConstantChunkScheduler::init() {

    exclTrans.resize(sim->getTotalObj());
    inclTrans.resize(sim->getTotalObj());
    sizeO.resize(sim->getTotalObj(), 0);
    sizeT.resize(sim->getTotalTrans(), make_pair(0, 0));
    for(int i = 0; i < sizeT.size(); i ++){
        sizeT[i].first = i;
        sizeT[i].second = 0;
    }
}

bool ConstantChunkScheduler::acquire(int tid, int oid, bool excl) {

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
        inclTrans[oid].push_back(&sizeT[tid]);
        updateO(oid, 1 + sizeT[tid].second);
    }

    return false;
}

void ConstantChunkScheduler::release(int tid, int oid) {

    sim->getObj(oid).releaseBy(tid);
    updateT(tid, -sizeO[oid]);

    if (sim->getObj(oid).getStatus() == Object::FREE) {
        if (!exclTrans[oid].empty() || !inclTrans[oid].empty()) {
            sim->addToAssign(oid);
        }
    }
}


const std::set<int> ConstantChunkScheduler::assign(int oid) {

    std::set<int> assigned;

    std::sort(exclTrans[oid].begin(), exclTrans[oid].end(), myComparater);
    std::sort(inclTrans[oid].begin(), inclTrans[oid].end(), myComparater);

    unsigned int firstChunkSize = 0;
    unsigned int restChunkSize = 0;
    for (auto it = inclTrans[oid].begin(); it < inclTrans[oid].begin() + ChunkSize; it ++)
        firstChunkSize += (*it)->second;
    for (auto it = inclTrans[oid].begin()+ ChunkSize; it < inclTrans[oid].end() + ChunkSize; it ++)
        restChunkSize += (*it)->second;

    unsigned int totalWriteSize = 0;
    for (auto it = exclTrans[oid].begin()+ ChunkSize; it < exclTrans[oid].end() + ChunkSize; it ++)
        totalWriteSize += (*it)->second;

    double latencyW = totalWriteSize + firstChunkSize + restChunkSize;
    double latencyR = f(ChunkSize) * (restChunkSize + totalWriteSize) + firstChunkSize;

    if (latencyR > latencyW) { //assign the lock to write
        int trans = exclTrans[oid][0]->first;

        updateO(oid, - exclTrans[oid][0]->second - 1);

        sim->getTrans(trans).grantLock();

        updateT(trans, sizeO[oid]); // grant lock BEFORE this update ! ! !

        assigned.insert(trans);
        exclTrans[oid].erase(exclTrans[oid].begin());

        sim->getObj(oid).addOwner(assigned, true);
    }
    else {  //assign the lock to reads
        updateO(oid, -firstChunkSize);

        for (auto itr = inclTrans[oid].begin(); itr != inclTrans[oid].end(); ++itr) {
            int trans = (*itr)->first;
            sim->getTrans(trans).grantLock();
            updateT(trans, sizeO[oid]); // grant lock BEFORE this update ! ! !
            assigned.insert(trans);
        }

        inclTrans[oid].erase(inclTrans[oid].begin(), inclTrans[oid].begin()+firstChunkSize);

        sim->getObj(oid).addOwner(assigned, false);
    }

    return assigned;
}

int ConstantChunkScheduler::getTime() { return sim->getTime(); }

void ConstantChunkScheduler::updateO(int oid, int delta) {
    sizeO[oid] += delta;
    for (auto itr = sim->getObj(oid).getOwner().begin(); itr != sim->getObj(oid).getOwner().end(); ++itr) {
        updateT(*itr, delta);
    }
}

void ConstantChunkScheduler::updateT(int tid, int delta) {
    sizeT[tid].second += delta;
    int oid = sim->getTrans(tid).getBlockBy();
    if (oid > 0)
    {
        updateO(oid, delta);
    }
}
