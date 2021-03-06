#include "transaction.h"
#include "action.h"
#include "scheduler.h"
#include <iostream>

int Transaction::proceed()
{
//    std::cerr << "transaction " << id << " proceeds to curTime " << curTime << std::endl;
//    std::cerr << "next event is at curTime " << acts[cursor].time << std::endl;
    if (curTime++ < acts[cursor].time)
    { 
        return RUNNING;
    }

//    std::cerr << "the event happens, which is" << acts[cursor].lock << std::endl;
    switch (acts[cursor].lock)
    {
        case Action::FINISH :
            endTime = sch->getTime();
            for (auto itr = holds.begin(); itr != holds.end(); ++itr)
            {
//                std::cerr << "transaction " << id << " releases lock #" << *itr << std::endl;
                sch->release(id, *itr);
            }
//            std::cerr << "transaction " << id << " finishes" << std::endl;
            return FINISH;
        case Action::START :
            ++cursor;
            return RUNNING;
        default :
            if (sch->acquire(id, acts[cursor].lock, acts[cursor].excl))
            {
//                std::cerr << "transaction " << id << " successfully get lock #" << acts[cursor].lock << std::endl;
                holds.push_back(acts[cursor].lock);
                ++cursor;

                return RUNNING;
            }
            else
            {
//                std::cerr << "transaction " << id << " is blocked by lock #" << acts[cursor].lock << std::endl;
                blockBy = acts[cursor].lock;
                ++cursor;
                return BLOCKED;
            }
    }
}

void Transaction::grantLock()
{
//    std::cerr << "grant lock #" << blockBy << " to transaction " << id << std::endl;

    holds.push_back(blockBy);
    blockBy = -1;
}


