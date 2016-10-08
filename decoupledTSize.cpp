#include "decoupledTSize.h"
#include "scheduler.h"
#include "object.h"
#include "simulator.h"
#include "transaction.h"

#include <vector>
#include <set>
#include <iostream>

DecoupledTSize::DecoupledTSize() : Scheduler() {}

DecoupledTSize::DecoupledTSize(Simulator* sim) : sim(sim) {}

void DecoupledTSize::init()
{
    exclTrans.resize(sim->getTotalObj());
    inclTrans.resize(sim->getTotalObj());
    sizeO.resize(sim->getTotalObj(), 0);
    sizeT.resize(sim->getTotalTrans(), 0);
}

bool DecoupledTSize::acquire(int tid, int oid, bool excl)
{
    int status = sim->getObj(oid).getStatus();

    if (status == Object::FREE)
    {
        std::set<int> trans;
        trans.insert(tid);
        sim->getObj(oid).addOwner(trans.excl);
        return true;
    }

    if (excl)
    {
        exclTrans[oid].push_back(tid);
        updateO(oid, 1 + sizeT[tid]);
    }
    else
    {
        inclTrans[oid].push_back(tid);
        updateO(oid, 1 + sizeT[tid]);
    }

    return false;
}

void DecoupledTSize::release(int tid, int oid)
{
    sim->getObj(oid).releaseBy(tid);
    sizeT[tid] -= sizeO[oid];

    if (sim->getObj(oid).getStatus() == Object::FREE)
    {
        if (!exclTrans[oid].empty() || !inclTrans[oid].empty())
        {
            sim->addToAssign(oid);
        }
    }
}

const std::set<int> DecoupledTSize::assign(int oid)
{
    std::set<int> assigned;

    int maxSize = -1;
    std::vector<int>::iterator maxpos;

    for (int i = 0; i < exclTrans[oid].size(); ++i)
    {
        if (sizeT[exclTrans[i]] > maxSize)
        {
            maxSize = sizeT[exclTrans[i]];
            maxpos = i;
        }
    }

    int inclSize = 0;
    for (auto itr = inclTrans[oid].begin(); itr != inclTrans[oid].end(); ++itr)
    {
        inclSize += sizeT[*itr] + 1;
    }
    --inclSize;

    if (inclSize < maxSize)
    {
        int trans = exclTrans[maxpos];

        updateO(oid, -maxSize - 1);
        updateT(trans, sizeO[oid]);

        sim->getTrans(trans).grantLock();

        assigned.insert(trans);
        exclTrans.erase(exclTrans.begin() + maxpos);

        sim->getObj(oid).addOwner(assigned, true);
    }
    else
    {
        updateO(oid, -inclSize - 1);
        for (auto itr = inclTrans[oid].begin(); itr != inclTrans[oid].end(); ++itr)
        {
            updateT(*itr, sizeO[oid]);
            assigned.insert(*itr);
        }

        inclTrans[oid].clear();

        sim->getObj(oid).addOwner(assigned, false);
    }

    return assigned;
}

int DecoupledTSize::getTime() { return sim->getTime(); }

void DecoupledTSize::updateO(int oid, int delta)
{
    sizeO[oid] += delta;
    for (auto itr = sim->getObj(oid).getOwner().begin(); itr != sim->getObj(obj).getOwner().end(); ++itr)
    {
        updateT(*itr, delta);
    }
}

void DecoupledTSize::updateT(int tid, int delta)
{
    sizeT[tid] += delta;
    int oid = sim->getTrans(tid).getBlockBy();
    if (oid > 0)
    {
        updateO(oid, delta);
    }
}
