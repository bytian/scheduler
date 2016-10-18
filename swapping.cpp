#include "swapping.h"
#include "scheduler.h"
#include "object.h"
#include "simulator.h"
#include "transaction.h"

#include <vector>
#include <set>
#include <iostream>

Swapping::Swapping() : Scheduler() {}

Swapping::Swapping(Simulator* sim) : sim(sim) {}

void Swapping::init()
{
    schedule.resize(sim->getTotalObj());
    inclTrans.resize(sim->getTotalObj());
    releaseTime.resize(sim->getTotalObj(), 0);
    finishTime.resize(sim->getTotalTrans(), 0);
}

bool Swapping::acquire(int tid, int oid, bool excl)
{
    int latency = 0;

    int status = sim->getObj(oid).getStatus();
    if (finishTime[tid] == 0)
    {
        updateT(tid, 1, latency);
    }

    if (status == Object::FREE && schedule[oid].empty())
    {
        std::set<int> trans;
        trans.insert(tid);
        sim->getObj(oid).addOwner(trans, excl);
        updateO(oid, latency);
        return true;
    }

    if (excl)
    {
        schedule[oid].push_back(tid);
        if (schedule[oid].size() == 1)
        {
            updateT(tid, releaseTime[oid], latency);
        }
        else 
        {
            int trans = schedule[oid][schedule[oid].size() - 2];
            if (trans == -1)
            {
                int time = finishTime[inclTrans[oid][0]];
                updateT(tid, time, latency);
            }
            else
            {
                int time = finishTime[trans];
                updateT(tid, time, latency);
            }
        }
    }
    else
    {
        if (inclTrans[oid].empty())
        {
            schedule[oid].push_back(-1);
            inclTrans[oid].push_back(tid);
            if (schedule[oid].size() == 1)
            {
                updateT(tid, releaseTime[oid], latency);
            }
            else
            {
                int trans = schedule[oid][schedule[oid].size() - 2];
                int time = finishTime[trans];
                updateT(tid, time, latency);
            }
        }
        else
        {
            inclTrans[oid].push_back(tid);
            int time = finishTime[inclTrans[oid][0]];
            updateT(tid, time - finishTime[tid], latency);
        }
    }

    swapFrom(oid);
}

void Swapping::release(int tid, int oid)
{
    int latency = 0;

    finishTime[tid] = 0;
    sim->getObj(oid).releaseBy(tid);
    updateO(oid, latency);

    if (sim->getObj(oid).getStatus() == Object::FREE)
    {
        if (!schedule[oid].empty())
        {
            sim->addToAssign(oid);
        }
    }
    swapFrom(oid);
}

const std::set<int> Swapping::assign(int oid)
{
    int latency = 0;

    int trans = schedule[oid].front();
    std::set<int> assigned;

    if (trans == -1)
    {
        for (auto itr = inclTrans[oid].begin(); itr != inclTrans[oid].end(); ++itr)
        {
            assigned.insert(*itr);
        }
        inclTrans[oid].clear();
    }
    else
    {
        assigned.insert(trans);
    }
    schedule[oid].erase(schedule[oid].begin());
    sim->getObj(oid).addOwner(assigned, (trans != -1));

    releaseTime[oid] = 1;

    swapFrom(oid);
}

int Swapping::getTime() { return sim->getTime(); }

void Swapping::updateO(int oid, int& latency)
{
    int maxTime = releaseTime[oid];
    for (auto itr = sim->getObj(oid).getOwner().begin(); itr != sim->getObj(oid).getOwner().end(); ++itr)
    {
        if (finishTime[*itr] > maxTime)
            maxTime = finishTime[*itr];
    }

    if (maxTime == releaseTime[oid])
    {
        return;
    }

    int delta = maxTime - releaseTime[oid];
    releaseTime[oid] = maxTime;
    for (auto itr = schedule[oid].begin(); itr != schedule[oid].end(); ++itr)
    {
        updateT(*itr, delta, latency);
    }
    for (auto itr = inclTrans[oid].begin(); itr != inclTrans[oid].end(); ++itr)
    {
        updateT(*itr, delta, latency);
    }

}

void Swapping::updateT(int tid, int delta, int& latency)
{
    latency += delta;
    finishTime[tid] += delta;
    for (auto itr = sim->getTrans(tid).getHolds().begin(); itr != sim->getTrans(tid).getHolds().end(); ++itr)
    {
        updateO(*itr, latency);
    }
}

void Swapping::swapFrom(int oid)
{
    int cnt = 0;
    while (cnt < sim->getTotalObj())
    {
        bool flag = swapO(oid);
        if (flag)
        {
            cnt = 0;
        }
        else
        {
            ++cnt;
        }
        oid = (oid + 1) % (sim->getTotalObj());
    }
}

bool Swapping::swapO(int oid)
{
    if (schedule[oid].size() < 2)
        return false;

    bool flag = false;
    for (int i = schedule[oid].size() - 2; i > 0; --i)
    {
        flag = swap(oid, i);
    }
    return flag;
}

bool Swapping::swap(int oid, int x)
{
    int latency = 0;

    if (schedule[oid][x] == -1)
    {
        for (int i = 0; i < inclTrans[oid].size(); ++i)
        {
            updateT(inclTrans[oid][i], 1, latency);
        }
    }
    else
    {
        updateT(schedule[oid][x], 1, latency);
    }

    if (schedule[oid][x + 1] == -1)
    {
        for (int i = 0; i < inclTrans[oid].size(); ++i)
        {
            updateT(inclTrans[oid][i], -1, latency);
        }
    }
    else
    {
        updateT(schedule[oid][x + 1], -1, latency);
    }

    if (latency < 0)
    {
        int tmp = schedule[oid][x];
        schedule[oid][x] = schedule[oid][x + 1];
        schedule[oid][x + 1] = tmp;
        return true;
    }

    if (schedule[oid][x] == -1)
    {
        for (int i = 0; i < inclTrans[oid].size(); ++i)
        {
            updateT(inclTrans[oid][i], -1, latency);
        }
    }
    else
    {
        updateT(schedule[oid][x], -1, latency);
    }

    if (schedule[oid][x + 1] == -1)
    {
        for (int i = 0; i < inclTrans[oid].size(); ++i)
        {
            updateT(inclTrans[oid][i], 1, latency);
        }
    }
    else
    {
        updateT(schedule[oid][x + 1], 1, latency);
    }

    return false;
}
