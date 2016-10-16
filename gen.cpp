#include <iostream>
#include <cstring>
#include <random>
#include <cstdlib>
#include <vector>
#include "action.h"
#include "RandomSeqGen.cpp"

using namespace std;


//const double FINPROB = 0.35;

vector<Action> genTrans(RandomSeqGen* randGen, default_random_engine& genA,exponential_distribution<double> poiA, int NUM_TRAIL, int LOCK_TYPE_PROB, int USE_POWER_LAW)
{
    int time = 0;
    vector<Action> act;
    act.push_back(Action(time, Action::START, true));

    vector<int> objSeq;

    if (USE_POWER_LAW){
        objSeq = randGen->getPowerLawSeq(NUM_TRAIL);
    } else {
        objSeq = randGen->getUniformSeq(NUM_TRAIL);
    }

    int old_obj = -1;

    for(auto& obj: objSeq){
        int dtime = 1 + poiA(genA);
        if (old_obj != obj){
            double t = rand() / (double) RAND_MAX;
            bool lock_type = t <= LOCK_TYPE_PROB;
            act.push_back(Action(time += dtime, obj, lock_type));
            old_obj = obj;
        } else {
            time = ((act.begin() + act.size() - 1)->time += dtime);
        }

    }
    int dtime = 1 + poiA(genA);
    act.push_back(Action(time + dtime, Action::FINISH, true));

    return act;
}

int main(int argc, const char * argv[]) {
    srand (time(NULL));
    int NUM_OBJ = atoi(argv[1]);
    int NUM_TRAN = atoi(argv[2]);
    int NUM_TRAIL = atoi(argv[3]);

    double NEWT = atof(argv[4]);
    double NEWA = atof(argv[5]);

    bool USE_POWER_LAW;
    if (strcmp(argv[6], "true") == 0)
        USE_POWER_LAW = true;
    else
        USE_POWER_LAW = false;

    double LOCK_TYPE_PROB = atof(argv[7]);

    double POWER_LAW_PARAM = atof(argv[8]);

    //cout << NUM_OBJ << " " << NUM_TRAN << " " << NUM_TRAIL << " " << NEWT << " " << NEWA << " " << USE_POWER_LAW << " " << LOCK_TYPE_PROB << " " << POWER_LAW_PARAM << endl;

    // int NUM_OBJ = 10;
    // int NUMTRAN = 20000;
    // int NUM_TRAIL = 10;

    //double NEWT = 0.032;
    //double NEWA = 0.005;

    //double LOCK_TYPE_PROB = 0.75;

    //bool USE_POWER_LAW = true;

    //double POWER_LAW_PARAM = 1.5;

    default_random_engine genT((unsigned) time(0));
    exponential_distribution<double> poiT(NEWT);
    default_random_engine genA((unsigned) time(0));
    exponential_distribution<double> poiA(NEWA);

    cout << NUM_OBJ << ' ' << NUM_TRAN << endl;

    RandomSeqGen* randGen = new RandomSeqGen(NUM_OBJ, POWER_LAW_PARAM);

    int startTime = 0;

    for (int i = 0; i < NUM_TRAN; ++i)
    {
        vector<Action> acts = genTrans(randGen, genA, poiA, NUM_TRAIL, LOCK_TYPE_PROB, USE_POWER_LAW);
        cout << startTime << ' ' << acts.size() << endl;
        for (int j = 0; j < acts.size(); ++j) {
            cout << acts[j].time << ' ' << acts[j].lock << ' ' << acts[j].excl << endl;
        }
        startTime += poiT(genT);
    }
    return 0;
}

