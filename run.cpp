#include "scheduler.h"
#include "simulator.h"
#include "transaction.h"
#include <iostream>
using namespace std;

int main()
{
    Simulator sim;

    srand((unsigned) time(0));

    cerr << "begin" <<endl;
    sim.inputTrans();

    cerr << "input" << endl;
    sim.run();

    long long tot = 0;
    long long waitt = 0;
    for (int i = 0; i < sim.getTotalTrans(); ++i)
    {
//        cerr << i << ": " << sim.getTrans(i).latency() << endl;
        tot += sim.getTrans(i).latency();
        waitt += sim.getTrans(i).getWaitTime();
    }
    cerr << "run" << endl;
    cout << tot / (double) sim.getTotalTrans() << " "; //average latency:
    cout << waitt / (double) sim.getTotalTrans() << endl;  //average wait time:
    return 0;
}
