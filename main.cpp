#include "main.h"
#include <vector>
#include <ctime>
#include <string>
#include <iostream>
#define NUMATTRS 13
    
using namespace std;

int main() {
    clock_t start, end;
    vector<instance> dataset;
    schema sc(NUMATTRS); 
    parseSchema("btrain.csv", sc);
    parseInstances("btrain.csv", dataset);
    // generate a decision tree
    start = clock();
    decisionTree dt(sc); dt.train(dataset); 
    end = clock();
    cout << "time to train the tree: " << double(end - start) / CLOCKS_PER_SEC << endl;
    start = clock();
    cout << "correct rate: " << dt.validate(dataset) << endl;
    end = clock();
    cout << "time to validate: " << double(end - start) / CLOCKS_PER_SEC << endl;
    return 0;
}

