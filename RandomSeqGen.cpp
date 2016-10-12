//
// Created by Jamie Wu on 10/11/16.
//

#include <vector>
#include <math.h>
#include <random>
#include <set>
#include <iostream>

using namespace std;

class RandomSeqGen {
private:
    int totalElementNum;
    vector<int> powerLawPool;

public:
    RandomSeqGen(int elementNumn, double gama);
    ~RandomSeqGen(){};
    set<int> getUniformSeq(int n);
    set<int> getPowerLawSeq(int n);
};

RandomSeqGen::RandomSeqGen(int elementNum, double gama) {
    totalElementNum = elementNum;
    for (int i = 1; i <= totalElementNum; i++){
        for(int j = 0; j < pow(i, gama); j ++){
            powerLawPool.push_back(i);
        }
    }
}

set<int> RandomSeqGen::getPowerLawSeq(int n) {
    set<int> sequence;
    for(int i = 0; i < n; i ++){
        int r = rand() % powerLawPool.size();
        sequence.insert(powerLawPool[r]);
    }
    return sequence;
}

set<int> RandomSeqGen::getUniformSeq(int n) {
    set<int> sequence;
    for(int i = 0; i < n; i ++){
        int r = rand() % n + 1;
        sequence.insert(r);
    }
    return  sequence;
}


int main () {
    srand (time(NULL));
    RandomSeqGen* randGen = new RandomSeqGen(10, 1.5);

    for (int i = 0; i < 10; i++){
        set<int> result;

        if (i < 5){
            result = randGen->getUniformSeq(8);
            cout << "Unifirm: ";
        } else {
            result = randGen->getPowerLawSeq(8);
            cout << "PowerLaw: ";
        }
        for(auto& it: result){
            cout << it << " ";
        }
        cout << endl;
    }
    delete randGen;
    return  1;
}