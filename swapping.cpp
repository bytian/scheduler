#include "swapping.h"
#include "scheduler.h"
#include "object.h"
#include "simulator.h"
#include "transaction.h"

#include <vector>
#include <set>
#include <iostream>

using std::cerr;
using std::endl;

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
    //cerr << "in acquire" << endl;
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
        //cerr << "out acquire" << endl;
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
    //cerr << "out acquire" << endl;
    return false;
}

void Swapping::release(int tid, int oid)
{
    //cerr << "in release" << tid << ' ' << oid << endl;
    int latency = 0;

    finishTime[tid] = 0;
    sim->getObj(oid).releaseBy(tid);
    updateO(oid, latency);

    if (sim->getObj(oid).getStatus() == Object::FREE)
    {
        if (!schedule[oid].empty())
        {
            //cerr << "lock #" << oid << " is ready to be assigned" << endl;
            sim->addToAssign(oid);
        }
    }
    swapFrom(oid);
    //cerr << "out release" << endl;
}

const std::set<int> Swapping::assign(int oid)
{
    //cerr << "in assign" << endl;
    int latency = 0;

    int trans = schedule[oid].front();
    std::set<int> assigned;

    if (trans == -1)
    {
        for (auto itr = inclTrans[oid].begin(); itr != inclTrans[oid].end(); ++itr)
        {
            assigned.insert(*itr);
            sim->getTrans(*itr).grantLock();
        }
        inclTrans[oid].clear();
    }
    else
    {
        assigned.insert(trans);
        sim->getTrans(trans).grantLock();
    }
    schedule[oid].erase(schedule[oid].begin());
    sim->getObj(oid).addOwner(assigned, (trans != -1));

    releaseTime[oid] = 1;

    swapFrom(oid);
    //cerr << "out assign" << endl;

    return assigned;
}

int Swapping::getTime() { return sim->getTime(); }

void Swapping::updateO(int oid, int& latency)
{
//    cerr << "in update O " << oid << ' ' << latency << endl;
    int maxTime = 0;
    for (auto itr = sim->getObj(oid).getOwner().begin(); itr != sim->getObj(oid).getOwner().end(); ++itr)
    {
        //cerr << *itr << endl;
        if (finishTime[*itr] > maxTime)
            maxTime = finishTime[*itr];
    }

    if (maxTime == releaseTime[oid])
    {
//        cerr << "out update O" <<endl;
        return;
    }

    int delta = maxTime - releaseTime[oid];
    releaseTime[oid] = maxTime;
    for (auto itr = schedule[oid].begin(); itr != schedule[oid].end(); ++itr)
    {
        if (*itr == -1) continue;
        updateT(*itr, delta, latency);
    }
    for (auto itr = inclTrans[oid].begin(); itr != inclTrans[oid].end(); ++itr)
    {
        updateT(*itr, delta, latency);
    }
//    cerr << "out update O" << endl;
}

void Swapping::updateT(int tid, int delta, int& latency)
{
//    cerr << "in update T " << tid << ' ' << delta << ' ' << latency << endl;
    latency += delta;
    finishTime[tid] += delta;
    for (auto itr = sim->getTrans(tid).getHolds().begin(); itr != sim->getTrans(tid).getHolds().end(); ++itr)
    {
        updateO(*itr, latency);
    }
//    cerr << "out update T" << endl;
}

void Swapping::swapFrom(int oid)
{
    //cerr << "in swapFrom" << endl;
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
    //cerr << "out swapFrom" << endl;
}

bool Swapping::swapO(int oid)
{
    //cerr << "in swap O" << endl;
    if (schedule[oid].size() < 2)
    {
        //cerr << "out swap O" << endl;
        return false;
    }

    bool flag = false;
    for (int i = schedule[oid].size() - 2; i >= 0; --i)
    {
        flag = swap(oid, i);
    }
    //cerr << "out swap O" << endl;
    return flag;
}

bool Swapping::swap(int oid, int x)
{
//    cerr << "in swap " << oid << ' ' << x << endl;
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

//    cerr << "delta latency: " << latency << endl;

    if (latency < 0)
    {
        int tmp = schedule[oid][x];
        schedule[oid][x] = schedule[oid][x + 1];
        schedule[oid][x + 1] = tmp;
//        cerr << "out swap true" << endl;
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
//    cerr << "out swap false" << endl;

    return false;
}
