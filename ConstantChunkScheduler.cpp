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

ConstantChunkScheduler::ConstantChunkScheduler(Simulator* sim) : sim(sim) {chunkSize = 10;}

ConstantChunkScheduler::ConstantChunkScheduler(Simulator* sim, int chkSize) : sim(sim) { chunkSize = chkSize;}

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
    //cerr << "i am in assign " << chunkSize << endl;
    std::set<int> assigned;

    if (exclTrans[oid].size() == 0 and inclTrans[oid].size() == 0)
        return  assigned;

    std::sort(exclTrans[oid].begin(), exclTrans[oid].end(), myComparater);
    std::sort(inclTrans[oid].begin(), inclTrans[oid].end(), myComparater);

    unsigned int firstChunkSize = 0;
    unsigned int restChunkSize = 0;

    for (auto it = inclTrans[oid].begin(); it < inclTrans[oid].begin() + chunkSize and it < inclTrans[oid].end(); it ++)
        firstChunkSize += (*it)->second;
    //cerr << "I am here ..adf." << endl;
    if (chunkSize >= inclTrans[oid].size())
        restChunkSize = 0;
    else {
        for (auto it = inclTrans[oid].begin()+ chunkSize; it < inclTrans[oid].end(); it ++)
            restChunkSize += (*it)->second;
    }
    //cerr << "I am here ..." << endl;
    unsigned int totalWriteSize = 0;
    for (auto it = exclTrans[oid].begin(); it < exclTrans[oid].end(); it ++)
        totalWriteSize += (*it)->second;

    double latencyW = totalWriteSize + firstChunkSize + restChunkSize;
    double latencyR = f(chunkSize) * (restChunkSize + totalWriteSize) + firstChunkSize;
    //cerr << "I am here" << endl;
    if (exclTrans[oid].size() > 0 and (inclTrans[oid].size() == 0 or latencyR >= latencyW)) { //assign the lock to write
        //cerr << "I am in if first" << endl;
        int trans = exclTrans[oid][0]->first;
        //cerr << "I am here1" << endl;
        updateO(oid, - exclTrans[oid][0]->second - 1);

        sim->getTrans(trans).grantLock();

        updateT(trans, sizeO[oid]); // grant lock BEFORE this update ! ! !

        assigned.insert(trans);
        exclTrans[oid].erase(exclTrans[oid].begin());

        sim->getObj(oid).addOwner(assigned, true);
    }
    else if (inclTrans[oid].size() > 0 and (exclTrans[oid].size() == 0 or latencyR < latencyW)) {  //assign the lock to reads of the first chunk
        //cerr << "I am in if second" << endl;
        updateO(oid, -firstChunkSize);
        //cerr << "I am here2" << endl;
        for (auto itr = inclTrans[oid].begin(); itr != inclTrans[oid].end() and itr != inclTrans[oid].begin()+chunkSize; ++itr) {
            int trans = (*itr)->first;
            sim->getTrans(trans).grantLock();
            updateT(trans, sizeO[oid]); // grant lock BEFORE this update ! ! !
            assigned.insert(trans);
        }
        //cerr << "I am here222" << endl;
        if (chunkSize >= inclTrans[oid].size())
            inclTrans[oid].erase(inclTrans[oid].begin(), inclTrans[oid].end());
        else
            inclTrans[oid].erase(inclTrans[oid].begin(), inclTrans[oid].begin()+chunkSize);

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
