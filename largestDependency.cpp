#include "largestDependency.h"
#include "scheduler.h"
#include "object.h"
#include "simulator.h"
#include "transaction.h"

#include <vector>
#include <set>
#include <iostream>
#include <deque>

LargestDependency::LargestDependency() : Scheduler() {}

LargestDependency::LargestDependency(Simulator* sim) : sim(sim) {}

void LargestDependency::init()
{
    exclTrans.resize(sim->getTotalObj());
    inclTrans.resize(sim->getTotalObj());
}

bool LargestDependency::acquire(int tid, int oid, bool excl)
{
    int status = sim->getObj(oid).getStatus();

    if (status == Object::FREE && exclTrans[oid].empty() && inclTrans[oid].empty())
    {
        std::set<int> trans;
        trans.insert(tid);
        sim->getObj(oid).addOwner(trans, excl);
        return true;
    }

    if (excl)
    {
        exclTrans[oid].push_back(tid);
    }
    else
    {
        inclTrans[oid].push_back(tid);
    }

    return false;
}

void LargestDependency::release(int tid, int oid)
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

const std::set<int> LargestDependency::assign(int oid)
{
    std::set<int> assigned;

    int maxSize = -1;
    std::vector<int>::iterator maxItr;

    for (auto itr = exclTrans[oid].begin(); itr != exclTrans[oid].end(); ++itr)
    {
        int size = getDepSize(itr, itr + 1);
        
        if (size > maxSize)
        {
            maxSize = size;
            maxItr = itr;
        }
    }

    int inclSize = getDepSize(inclTrans[oid].begin(), inclTrans[oid].end());

    if (inclSize < maxSize)
    {
        assigned.insert(*maxItr);
        sim->getTrans(*maxItr).grantLock();
        exclTrans[oid].erase(maxItr);
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
    
    return assigned;
}

int LargestDependency::getTime() { return sim->getTime(); }

int LargestDependency::getDepSize(std::vector<int>::iterator begin, std::vector<int>::iterator end)
{
    int cnt = 0;

    std::vector<bool> f(sim->getTotalTrans(), false);
    std::vector<bool> g(sim->getTotalObj(), false);

    std::deque<int> q(begin, end);

    for (auto itr = begin; itr != end; ++itr)
    {
        f[*itr] = true;
    }

    while (!q.empty())
    {
        int t = q.front();
        q.pop_front();
        ++cnt;

        for (auto oitr = sim->getTrans(t).getHolds().begin(); oitr != sim->getTrans(t).getHolds().end(); ++oitr)
        {
            if (g[*oitr]) continue;
            g[*oitr] = true;
            for (auto titr = exclTrans[*oitr].begin(); titr != exclTrans[*oitr].end(); ++titr)
            {
                if (f[*titr]) continue;
                f[*titr] = true;
                q.push_back(*titr);
            }
        }
    }

    return cnt;
}
