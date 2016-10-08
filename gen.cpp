#include <iostream>
#include <random>
#include <cstdlib>
#include <vector>
#include "action.h"
using namespace std;

const int NUMLOCK = 10;
const int NUMTRAN = 20000;
const int NEWT = 100;
const int NEWA = 100;
const double FINPROB = 0.3;

default_random_engine genT;
poisson_distribution<int> poiT(NEWT);
default_random_engine genA;
poisson_distribution<int> poiA(NEWA);

vector<Action> genTrans()
{
    int time = 0;
    vector<Action> act;
    act.push_back(Action(time, Action::START, true));
    int lockFrom = 0;
    while (true)
    {
        int dtime = 1 + poiA(genA);

        
        double r = rand() / (double) RAND_MAX;

        if (lockFrom == NUMLOCK) r = 0.;

        if (r < 0.4) // finish
        {
            act.push_back(Action(time += dtime, Action::FINISH, true));
            break;
        }
        else // acquire a lock (for now assume all locks are EXCLUSIVE)
        {
            // only locks from lockFrom can be acquired
            int lock = rand() % (NUMLOCK - lockFrom) + lockFrom; // each lock has equal probability to be acquired
            act.push_back(Action(time += dtime, lock, true));
            lockFrom = lock + 1;
        }
    }

    return act;
}

int main()
{
    cout << NUMLOCK << ' ' << NUMTRAN << endl;

    int startTime = 0;
    for (int i = 0; i < NUMTRAN; ++i)
    {
        vector<Action> acts = genTrans();
        cout << startTime << ' ' << acts.size() << endl;
        for (int j = 0; j < acts.size(); ++j)
        {
            cout << acts[j].time << ' ' << acts[j].lock << ' ' << acts[j].excl << endl;
        }
        startTime += poiT(genT);
    }
    return 0;
}

