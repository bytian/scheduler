#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <vector>
#include "action.h"
#include "scheduler.h"

class Transaction
{
public:
    static const int RUNNING = 0;
    static const int BLOCKED = 1;
    static const int FINISH = -1;
private:
    // id of the transaction
    int id;

    // start time of the transaction
    int startTime; 

    // sequence of actions taken, including starting, finishing and acquiring locks
    std::vector<Action> acts; 

    // next action to take
    int cursor;

    // end time of the transaction
    int endTime;

    // current time of the transaction
    int curTime;
    
    // the object the transaction is blocked by
    int blockBy;

    // the set of object the transaction holds a lock on
    std::vector<int> holds;

    // the scheduler
    Scheduler* sch;

public:
    /********************************************
     * Default constructor
     ********************************************/
    Transaction() {}

    /********************************************
     * Constructor
     ********************************************/
    Transaction(int id, int startTime, const std::vector<Action> &acts, Scheduler* sch) :
        id(id),
        startTime(startTime),
        acts(acts),
        cursor(0),
        endTime(-1),
        curTime(0),
        blockBy(-1),
        sch(sch) {}

    /********************************************
     * Execution of the transaction at current time
     * @ReturnValues
     * RUNNING : for still running
     * BLOCKED : for blocked due to acquiring a lock
     * FINISH : for finishing
     ********************************************/
    int proceed();

    /********************************************
     * Called when lock on object blockBy is 
     * assigned to the transaction
     ********************************************/
    void grantLock();

    /******************************************** 
     * The latency of the transaction
     * If not finished yet, return -1
     ********************************************/
    int latency() const
    { return (endTime < 0) ? -1 : (endTime - startTime); }

    /********************************************
     * Query the start time of the transaction
     * @ReturnValue
     * the start time of the transaction
     ********************************************/
    int getStartTime() { return startTime; }

    /********************************************
     * Query how long the transaction has been
     * actually running
     ********************************************/
    int getRunTime() { return curTime; }

    /********************************************
     * Query how long the transaction is blocked
     ********************************************/
    int getWaitTime() { return (endTime < 0) ? -1 : (endTime - startTime -  acts.back().time); }

    int getID() { return id; }

    int getBlockBy() { return blockBy; }

    std::vector<int>& getHolds() { return holds; }
};
#endif
