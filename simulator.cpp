#include "simulator.h"
#include "object.h"
#include "transaction.h"
#include "scheduler.h"
#include "action.h"
#include "fifo.h"

#include <set>
#include <vector>
#include <iostream>

#define SIM_SCHEDULER FIFO

Simulator::Simulator()
{
    sch = new SIM_SCHEDULER(this);
}

Simulator::~Simulator()
{
    delete sch;
}

void Simulator::inputTrans()
{
    int n, m;
    std::cin >> n >> m;

    for (int i = 0; i < m; ++i)
    {
        obj.push_back(Object(i));
    }

    for (int i = 0; i < n; ++i)
    {
        int startTime, k;
        std::cin >> startTime >> k;
        std::vector<Action> acts(k);

        for (int j = 0; j < k; ++j)
        {
            int time, act;
            bool excl;
            std::cin >> time >> act >> excl;
            acts.push_back(Action(time, act, excl));
        }

        trans.push_back(Transaction(i, startTime, acts, sch));
    }
}

void Simulator::run()
{
    while (cursor < trans.size())
    {
        getNew();
        
        for (auto itr = to_assign.begin(); itr != to_assign.end(); ++itr)
        {
            assign(*itr);
        }
        to_assign.clear();

        proceed();

        ++clock;
    }
}

void Simulator::getNew()
{
    while (clock >= trans[cursor].getStartTime())
    {
        running.insert(cursor++);
    }
}

void Simulator::assign(int oid)
{
    auto assigned = sch->assign(oid);
    for (auto itr = assigned.begin(); itr != assigned.end(); ++itr)
    {
        running.insert(*itr);
    }
}

void Simulator::proceed()
{
    for (auto itr = running.begin(); itr != running.end(); )
    {
        int result = trans[*itr].proceed();
        if (result == Transaction::BLOCKED || result == Transaction::FINISH)
        {
            itr = running.erase(itr);
        }
        else
            ++itr;
    }
}

Transaction& Simulator::getTrans(int tid)
{
    return trans[tid];
}

Object& Simulator::getObj(int oid)
{
    return obj[oid];
}

#undef SIM_SCHEDULER
