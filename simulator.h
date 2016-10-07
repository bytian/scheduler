#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "object.h"
#include "scheduler.h"

#include <set>
#include <vector>


class Action;
class Transaction;

class Simulator
{
private:
    // current time
    int clock = 0;

    // the chosen scheduler
    Scheduler* sch;

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

    // number of transactions that have finished
    int finish = 0;
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
     * Constructor
     ******************************************/
    Simulator();

    /******************************************
     * Destructor
     ******************************************/
    ~Simulator();

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
     * Add objects that are just released and
     * that is blocking some transactions to
     * the set to_assign
     ******************************************/
    void addToAssign(int oid);



    /******************************************
     * Get the information of a transactions
     * @Parameter
     * tid : the id of the transaction
     * @ReturnValue
     * the transaction
     ******************************************/
    Transaction& getTrans(int tid);

    /******************************************
     * Get the information of an object
     * @Parameter
     * oid : the id of the object
     * @ReturnValue
     * the object
     ******************************************/
    Object& getObj(int oid);

    /******************************************
     * Return the number of transactions in all
     ******************************************/
    int getTotalTrans() const { return trans.size(); }

    /******************************************
     * Return the number of objects in all
     ******************************************/
    int getTotalObj() const { return obj.size(); }

    /******************************************
     * Return current time
     ******************************************/
    int getTime() const { return clock; }
};
#endif
