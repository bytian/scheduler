#include "simulator.h"
#include "fifo.h"
#include <iostream>
using namespace std;

int main()
{
    Simulator sim;

    sim.inputTrans();
    sim.run();

    int tot = 0;
    for (int i = 0; i < sim.getTotalTrans(); ++i)
    {
        tot += sim.getTrans(i).latency();
    }
    cout << "average latency:" << tot / (double) sim.getTotalTrans() << endl;

    return 0;
}
