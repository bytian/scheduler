#include "simulator.h"
#include "object.h"
#include "transaction.h"
#include "scheduler.h"
#include "action.h"
#include "fifo.h"
#include "iostream"

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

    for (int i = 0; i < n; ++i)
    {
        obj.push_back(Object(i));
    }

    for (int i = 0; i < m; ++i)
    {
        int k;
        std::cin >> k;
        std::vector<Action> acts;

        for (int j = 0; j < k; ++j)
        {
            int time, act;
            bool excl;
            std::cin >> time >> act >> excl;
            acts.push_back(Action(time, act, excl));
        }

        trans.push_back(Transaction(i, acts[0].time, acts, sch));
    }

    sch->init();
}

void Simulator::run()
{
    while (true)
    {
        if (clock % 100 == 0) std::cerr << clock << std::endl;

        getNew();

        // std::cerr << "\tgetnew" << std::endl;
        
        for (auto itr = to_assign.begin(); itr != to_assign.end(); ++itr)
        {
            assign(*itr);
        }
        to_assign.clear();

        // std::cerr << "\tassign" << std::endl;

        proceed();

        // std::cerr << "\tproceed" << std::endl;

        ++clock;

        if (cursor >= trans.size() && to_assign.empty() && running.empty())
            break;
        // std::cerr << "\t" << cursor << std::endl;
    }
}

void Simulator::getNew()
{
    while (cursor < trans.size() && clock >= trans[cursor].getStartTime())
    {
        // std::cerr << "\t\tT" << cursor << " " << trans[cursor].getStartTime() << std::endl;
        running.insert(cursor++);
    }
    // std::cerr << cursor << ' ' << trans[cursor].getStartTime() << std::endl;
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
    std::vector<int> removed;

    for (auto itr = running.begin(); itr != running.end(); ++itr)
    {
        int result = trans[*itr].proceed();
        if (result == Transaction::BLOCKED || result == Transaction::FINISH)
        {
            removed.push_back(*itr);
        }
    }

    for (int i = 0; i < removed.size(); ++i)
    {
        running.erase(removed[i]);
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
