#ifndef ACTION_H
#define ACTION_H

class Action
{
public:
    static const int START = -2;
    static const int FINISH = -1;
public:
    // time of the action
    int time; 

    // -2 for starting
    // -1 for finishing 
    // >=0 for the lock acquired
    int lock; 

    // true if the acuiring an exclusive lock
    // false for an inclusive lock 
    // if not acquiring a lock, does not matter
    bool excl; 

public:
    /*********************************************
     * Default Constructor
     *********************************************/
    Action() {}

    /*********************************************
     * Constructor
     *********************************************/
    Action(int time, int lock, bool excl) : 
        time(time), 
        lock(lock), 
        excl(excl) {}
};

#endif
