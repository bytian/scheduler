#include "transaction.h"
#include "action.h"
#include "scheduler.h"

int Transaction::proceed()
{
    if (curTime++ < acts[cursor].time)
        return RUNNING;
    
    switch (acts[cursor].lock)
    {
        case Action::FINISH :
            endTime = sch->getTime();
            return FINISH;
        case Action::START :
            ++cursor;
            return RUNNING;
        default :
            if (sch->acquire(id, acts[cursor].lock, acts[cursor].excl))
            {
                holds.push_back(acts[cursor].lock);
                return RUNNING;
            }
            else
            {
                blockBy = acts[cursor].lock;
                return BLOCKED;
            }
    }
}

void Transaction::grantLock()
{
    holds.push_back(blockBy);
    blockBy = -1;
}


