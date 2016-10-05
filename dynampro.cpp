#include "dynampro.h"
#include "scheduler.h"
#include "object.h"
#include "transaction.h"
#include "simulator.h"

#include <cmath>
#include <set>

using namespace std;


static const double DynamPro::ALPHA = 3;

DynamPro::DynamPro() : Scheduler() {}

void DynamPro::init()
{
    exclTrans.resize(sim->getTotalObj());
    inclTrans.resize(sim->getTotalObj());
    TNumDep.resize(sim->getTotalTrans(), 0);
    ONumDep.resize(sim->getTotalObj(), 0);
}

// TODO
bool DynamPro::acquire(int tid, int oid, bool excl)
{
    
}

// TODO
void DynamPro::release(int tid, int oid)
{

}

// TODO
const std::set<int> assign(int oid)
{

}
















double DynamPro::f(int r)
{
    return log((double) r);
}
