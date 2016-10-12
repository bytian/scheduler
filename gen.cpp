#include <iostream>
#include <random>
#include <cstdlib>
#include <vector>
#include "action.h"
#include "RandomSeqGen.cpp"

using namespace std;

const int NUMLOCK = 10;
const int NUMTRAN = 20000;
const int NUM_TRAIL = 10;

const double NEWT = 0.032;
const double NEWA = 0.005;

const double LOCK_TYPE_PROB = 0.75;

const bool USE_POWER_LAW = true;

const double POWER_LAW_PARAM = 1.5;
//const double FINPROB = 0.35;

default_random_engine genT((unsigned) time(0));
exponential_distribution<double> poiT(NEWT);
default_random_engine genA((unsigned) time(0));
exponential_distribution<double> poiA(NEWA);

vector<Action> genTrans(RandomSeqGen* randGen)
{
    int time = 0;
    vector<Action> act;
    act.push_back(Action(time, Action::START, true));

    set<int> objSeq;

    if (USE_POWER_LAW){
        objSeq = randGen->getPowerLawSeq(NUM_TRAIL);
    } else {
        objSeq = randGen->getUniformSeq(NUM_TRAIL);
    }

    for(auto& obj: objSeq){
        int dtime = 1 + poiA(genA);

        double t = rand() / (double) RAND_MAX;
        bool lock_type = t <= LOCK_TYPE_PROB;

        act.push_back(Action(time += dtime, obj, lock_type));
    }
    int dtime = 1 + poiA(genA);
    act.push_back(Action(time += dtime, Action::FINISH, true));

    return act;
}

int main()
{
    cout << NUMLOCK << ' ' << NUMTRAN << endl;
    RandomSeqGen* randGen = new RandomSeqGen(NUMLOCK, POWER_LAW_PARAM);
    int startTime = 0;
    for (int i = 0; i < NUMTRAN; ++i)
    {
        vector<Action> acts = genTrans(randGen);
        cout << startTime << ' ' << acts.size() << endl;
        for (int j = 0; j < acts.size(); ++j)
        {
            cout << acts[j].time << ' ' << acts[j].lock << ' ' << acts[j].excl << endl;
        }
        startTime += poiT(genT);
    }
    return 0;
}

