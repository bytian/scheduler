#include "object.h"

#include <iostream>
#include <set>
#include <assert.h>

int Object::getStatus() const
{
    if (owner.empty())
        return FREE;
    if (status)
        return EXCL;
    return INCL;
}

void Object::addOwner(const std::set<int>& tid, bool excl)
{
    if (getStatus() == EXCL)
    {
        std::cerr << "Object #" << id << " is held by transaction " << *(owner.begin()) << " exclusively." << std::endl;
        std::cerr << "Cannot add more owner." << std::endl;
        std::cout << "Assertion failure." << std::endl;
        assert(false);
    }
    if (getStatus() == INCL && excl)
    {
        std::cerr << "Object #" << id << "is protected by an inclusive lock" << std::endl;
        std::cerr << "Cannot grant an exclusive lock on it." << std::endl;
        assert(false);
        std::cout << "Assertion failure." << std::endl;
    }
    for (auto itr = tid.begin(); itr != tid.end(); ++itr)
    {
        owner.insert(*itr);
    }
    status = excl;
}

void Object::releaseBy(int tid)
{
//    std::cerr << "t" << tid << "wish to release me, before which I have " << owner.size() << " owners" << std::endl;
    owner.erase(tid);
//    std::cerr << "now I have " << owner.size() << " owners" << std::endl;
}

const std::set<int>& Object::getOwner() const
{
    return owner;
}

