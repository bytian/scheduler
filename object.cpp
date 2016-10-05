#include "object.h"

#include <iostream>
#include <set>

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
    for (auto itr = tid.begin(); itr != tid.end(); ++itr)
    {
        owner.insert(*itr);
    }
    status = excl;
}

void Object::releaseBy(int tid)
{
    owner.erase(tid);
}

const std::set<int>& Object::getOwner() const
{
    return owner;
}

