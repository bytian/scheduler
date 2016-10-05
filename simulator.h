#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <set>
#include <vector>

#include "object.h"
#include "transaction.h"
#include "scheduler.h"
#include "action.h"
#include "fifo.h"

#define SIM_SCHEDULER FIFO 

class SIM_SCHEDULER;
class Object;

class Simulator
{
private:
    // current time
    int clock = 0;

    // the chosen scheduler
    SIM_SCHEDULER sch(SIM_SCHEDULER* psch);

    // transactions
    std::vector<Transaction> trans;

    // objects
    std::vector<Object> obj;

    // the set of lock ready to be assigned
    std::vector<int> to_assign;
    
    //set of running transactions
    std::set<int> running;

    // pointer to incoming transactions
    int cursor = 0;
private:
    /******************************************
     * Put new transactions into running
     ******************************************/
    void getNew();

    /******************************************
     * Assign a lock on object oid to a 
     * transaction waiting on it.
     * @Parameters
     * oid : the id of the object
     ******************************************/
    void assign(int oid);

    /******************************************
     * All running transactions take its action
     * in the following clock.
     ******************************************/
    void proceed();
public:
    /******************************************
     * Get information of the transactions and 
     * objects from standard input
     ******************************************/
    void inputTrans();

    /******************************************
     * Simulate the running of the transactions
     ******************************************/
    void run();

    /******************************************
     * Get the information of a transactions
     * @Parameter
     * tid : the id of the transaction
     * @ReturnValue
     * the transaction
     ******************************************/
    const Transaction& getTrans(int tid)
    { return trans[tid]; }

    /******************************************
     * Get the information of an object
     * @Parameter
     * oid : the id of the object
     * @ReturnValue
     * the object
     ******************************************/
    const Object& getObj(int oid)
    { return obj[oid]; }

    /******************************************
     * Return the number of transactions in all
     ******************************************/
    int getTotalTrans() { return trans.size(); }

    /******************************************
     * Return the number of objects in all
     ******************************************/
    int getTotalObj() { return obj.size(); }

    /******************************************
     * Return current time
     ******************************************/
    int getTime() { return clock; }
};
#endif
