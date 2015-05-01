#include "main.h"
#include <vector>
#include <ctime>
#include <string>
#include <iostream>
    
using namespace std;

int main() {
    clock_t start, end;
    schema sc; 
    vector<instance> train_dataset, validate_dataset, test_dataset;
    readSchema("btrain.csv", sc);
    readInstances("btrain.csv", train_dataset, false);
    readInstances("bvalidate.csv", validate_dataset, false);
    readInstances("btest.csv", test_dataset, true);
    writeAll("egege.csv", sc, test_dataset);
    // generate a decision tree
    start = clock();
    decisionTree dt(sc); dt.train(train_dataset); 
    end = clock();
    cout << "time to train the tree: " << double(end - start) / CLOCKS_PER_SEC << endl;
    start = clock();
    cout << "correct rate on validation set: " << dt.validate(validate_dataset) << endl;
    cout << "correct rate on train set: " << dt.validate(train_dataset) << endl;
    end = clock();
    cout << "time to validate: " << double(end - start) / CLOCKS_PER_SEC << endl;
    return 0;
}

