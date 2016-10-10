#include <iostream>
#include <random>
#include <cstdlib>
#include <vector>
#include "action.h"
using namespace std;

const int NUMLOCK = 10;
const int NUMTRAN = 20000;
<<<<<<< HEAD
const double NEWT = 0.1;
const double NEWA = 0.05;
const double FINPROB = 0.2;
const double LOCK_TYPE_PROB = 0.5;
=======
const double NEWT = 0.32;
const double NEWA = 0.005;
const double FINPROB = 0.3;
const double LOCK_TYPE_PROB = 1;
>>>>>>> 88a411c07a6c1d9d2edf3cb08176ac2c4dfc4aed

default_random_engine genT((unsigned) time(0));
exponential_distribution<double> poiT(NEWT);
default_random_engine genA((unsigned) time(0));
exponential_distribution<double> poiA(NEWA);

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
        double t = rand() / (double) RAND_MAX;
        bool lock_type = t <= LOCK_TYPE_PROB;

        if (lockFrom == NUMLOCK) r = 0.;

        if (r < FINPROB) // finish
        {
            act.push_back(Action(time += dtime, Action::FINISH, true));
            break;
        }
        else // acquire a lock (for now assume all locks are EXCLUSIVE)
        {
            // only locks from lockFrom can be acquired
            int lock = rand() % (NUMLOCK - lockFrom) + lockFrom; // each lock has equal probability to be acquired
            act.push_back(Action(time += dtime, lock, lock_type));
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

