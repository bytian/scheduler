#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <cstdlib>
#include <set>

//#include "simulator.h"

class Simulator;

class Scheduler
{
protected:
    Simulator* sim;
public:
    /**************************************************
     * Default constructor
     **************************************************/
    Scheduler();

    /**************************************************
     * Constructor
     **************************************************/
    Scheduler(Simulator* sim);

    /**************************************************
     * Initializer
     * Called when simulator reads in information of
     * transactions and objects
     **************************************************/
    virtual void init() = 0;

    /**************************************************
     * Called when transaction tid acquires a lock on
     * object oid.
     * @Parameters
     * tid : the id of the transaction
     * oid : the id of the object
     * excl : true if the acquired lock is exclusive
     *        false if otherwise
     * @ReturnValue
     * true : if the transaction is granted the lock
     **************************************************/
    virtual bool acquire(int tid, int oid, bool excl) = 0;
    
    /**************************************************
     * Called when transaction tid releases the lock no
     * object oid.
     * @Parameters
     * tid : the id of the transaction
     * oid : the id of the object
     **************************************************/
    virtual void release(int tid, int oid) = 0;

    /**************************************************
     * Called by the simulator at the beginning of a 
     * time unit to assign a lock on released objects
     * to transactions.
     * @Parameters
     * oid : the id of the object
     **************************************************/
    virtual const std::set<int> assign(int oid) = 0;

    /**************************************************
     * Return current system time
     **************************************************/
    virtual int getTime() = 0; 
};

#endif
