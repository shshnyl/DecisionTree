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
    // generate a decision tree
    start = clock();
    decisionTree dt(sc); dt.train(train_dataset); 
    end = clock();
    cout << "time to train the tree: " << double(end - start) / CLOCKS_PER_SEC << endl;
    cout << "correct rate on validation set: " << dt.validate(validate_dataset) << endl;
    cout << "correct rate on train set: " << dt.validate(train_dataset) << endl;
    for (int i = 0; i < test_dataset.size(); i++) {
        test_dataset[i].flag = dt.predict(test_dataset[i]);
    }
    writeAll("testresult.csv", sc, test_dataset);
    return 0;
}

