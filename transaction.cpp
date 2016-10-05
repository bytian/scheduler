#include "transaction.h"
#include "action.h"
#include "scheduler.h"
#include <iostream>

int Transaction::proceed()
{
    if (curTime++ < acts[cursor].time)
        return RUNNING;

    switch (acts[cursor].lock)
    {
        case Action::FINISH :
            endTime = sch->getTime();
            std::cerr << "F1" << ' ' << id << ' ' << holds.size() << std::endl;
            for (auto itr = holds.begin(); itr != holds.end(); ++itr)
            {
                sch->release(id, *itr);
            }
            return FINISH;
        case Action::START :
            ++cursor;
            return RUNNING;
        default :
            if (sch->acquire(id, acts[cursor].lock, acts[cursor].excl))
            {
                holds.push_back(acts[cursor].lock);
                ++cursor;

                return RUNNING;
            }
            else
            {
                blockBy = acts[cursor].lock;
                ++cursor;
                return BLOCKED;
            }
    }
}

void Transaction::grantLock()
{
    holds.push_back(blockBy);
    blockBy = -1;
}


