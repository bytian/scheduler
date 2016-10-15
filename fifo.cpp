#include "fifo.h"
#include "scheduler.h"
#include "object.h"
#include "simulator.h"
#include "transaction.h"

#include <vector>
#include <deque>
#include <set>
#include <cstdlib>
#include <iostream>

FIFO::FIFO() : Scheduler() {}

FIFO::FIFO(Simulator* sim) : sim(sim) {}

void FIFO::init()
{
    exclTrans.resize(sim->getTotalObj());
    inclTrans.resize(sim->getTotalObj());
    exclTime.resize(sim->getTotalObj());
    inclTime.resize(sim->getTotalObj(), -1);
}

bool FIFO::acquire(int tid, int oid, bool excl)
{
//    std::cerr << "acquire" << tid << ' ' << oid << ' ' << excl << std::endl;
    int status = sim->getObj(oid).getStatus();
//    std::cerr << "empty " << oid << ' '<< sim->getObj(oid).getOwner().empty() << '\t';
//    for (auto itr = sim->getObj(oid).getOwner().begin(); itr != sim->getObj(oid).getOwner().end(); ++itr)
//        std::cerr << *itr << ' ';
//    std::cerr << std::endl;
    
//    std::cerr << "status" << status << std::endl;
    if (status == Object::FREE)
    {
//        std::cerr << "G1" << std::endl;
//        std::cerr << "sizes: " << exclTrans.size() << ' ' << inclTrans.size() << std::endl;
        std::set<int> trans;
        trans.insert(tid);
        sim->getObj(oid).addOwner(trans, excl);
        return true;
    }

    if (excl)
    {
        exclTrans[oid].push_back(tid);
        exclTime[oid].push_back(sim->getTime());
    }
    else
    {
        inclTrans[oid].insert(tid);
        if (inclTime[oid] < 0)
            inclTime[oid] = sim->getTime();
    }

    return false;
}

void FIFO::release(int tid, int oid)
{
//    std::cerr << "F2 " << tid << ' ' << oid << std::endl;
    sim->getObj(oid).releaseBy(tid);
//    std::cerr << "F22" <<std::endl;
    if (sim->getObj(oid).getStatus() == Object::FREE)
    {
//        std::cerr << "F23" << std::endl;
        if (!exclTrans[oid].empty() || !inclTrans[oid].empty())
        {
            sim->addToAssign(oid);
        }
    }
//    std::cerr << "F25" << std::endl;
}

const std::set<int> FIFO::assign(int oid)
{
//    std::cerr << "\tassigning object " << oid << std::endl;
    std::set<int> assigned;

    if (inclTime[oid] < 0 || (!exclTrans[oid].empty() && inclTime[oid] > exclTime[oid].front()))
    {
        int trans = exclTrans[oid].front();
        sim->getTrans(trans).grantLock();

        assigned.insert(trans);
        exclTrans[oid].pop_front();
        exclTime[oid].pop_front();

        sim->getObj(oid).addOwner(assigned, true);
    }
    else
    {
        assigned = inclTrans[oid];
        for (auto itr = assigned.begin(); itr != assigned.end(); ++itr)
        {
            sim->getTrans(*itr).grantLock();
        }

        inclTime[oid] = -1;
        inclTrans[oid].clear();

        sim->getObj(oid).addOwner(assigned, false);
    }

    return assigned;
}

int FIFO::getTime() { return sim->getTime(); }

