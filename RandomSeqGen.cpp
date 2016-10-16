//
// Created by Jamie Wu on 10/11/16.
//

#include <vector>
#include <math.h>
#include <random>
#include <algorithm>
#include <iostream>

using namespace std;

class RandomSeqGen {
private:
    int totalElementNum;
    vector<int> powerLawPool;

public:
    RandomSeqGen(int elementNumn, double gama);
    ~RandomSeqGen(){};
    vector<int> getUniformSeq(int n);
    vector<int> getPowerLawSeq(int n);
};

RandomSeqGen::RandomSeqGen(int elementNum, double gama) {
    totalElementNum = elementNum;
    for (int i = 0; i <= totalElementNum-1; i++){
        for(int j = 0; j < pow(i+1, gama); j ++){
            powerLawPool.push_back(i);
        }
    }
}

vector<int> RandomSeqGen::getPowerLawSeq(int n) {
    vector<int> sequence;
    for(int i = 0; i < n; i ++){
        int r = rand() % powerLawPool.size();
        sequence.push_back(powerLawPool[r]);
    }
    sort(sequence.begin(), sequence.end());
    return sequence;
}

vector<int> RandomSeqGen::getUniformSeq(int n) {
    vector<int> sequence;
    for(int i = 0; i < n; i ++){
        int r = rand() % n;
        sequence.push_back(r);
    }
    sort(sequence.begin(), sequence.end());
    return  sequence;
}

/*
int main () {
    srand (time(NULL));
    RandomSeqGen* randGen = new RandomSeqGen(10, 1.5);

    for (int i = 0; i < 10; i++){
        vector<int> result;

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
*/
