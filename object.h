#ifndef OBJECT_H
#define OBJECT_H

#include <set>

class Object
{
public:
    static const int EXCL = -1;
    static const int INCL = 1;
    static const int FREE = 0;
private:
    // id of the object
    int id;

    // the set of transactions that currently hold an lock on the object
    std::set<int> owner;
    
    // true for exclusive lock
    bool status;
public:
    /**************************************
     * Default constructor
     **************************************/
    Object() {}


    /**************************************
     * Constructor
     **************************************/
    Object(int id) : 
        id(id),
        status(true) {}

    /**************************************
     * Query the status of the object
     * @ReturnValue
     * EXCL : an exclusive lock is held
     * INCL : an inclusive lock is held
     * FREE : no lock is held
     **************************************/
    int getStatus() const;

    /**************************************
     * The lock is assigned to the set of 
     * transactions 
     * @Parameters
     * tid : the id of the transactions
     **************************************/
    void addOwner(const std::set<int>& tid, bool excl);

    /**************************************
     * The lock is released by a transaction
     * @Parameters
     * tid : the id of the transaction
     **************************************/
    void releaseBy(int tid);

    /**************************************
     * Return the set of transactions that
     * hold a lock on the object
     **************************************/
    const std::set<int>& getOwner() const;
};

#endif
