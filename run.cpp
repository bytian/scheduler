#include "scheduler.h"
#include "simulator.h"
#include "transaction.h"
#include "fifo.h"
#include <iostream>
using namespace std;

int main()
{
    Simulator sim;

    sim.inputTrans();

    cerr << "input" << endl;
    sim.run();

    int tot = 0;
    for (int i = 0; i < sim.getTotalTrans(); ++i)
    {
        tot += sim.getTrans(i).latency();
    }
    cerr << "run" << endl;
    cout << "average latency:" << tot / (double) sim.getTotalTrans() << endl;

    return 0;
}
