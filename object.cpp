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
//    std::cerr << "t" << tid << "wish to release me, before which I have " << owner.size() << " owners" << std::endl;
    owner.erase(tid);
//    std::cerr << "now I have " << owner.size() << " owners" << std::endl;
}

const std::set<int>& Object::getOwner() const
{
    return owner;
}

