#include "main.h"
#include <vector>
#include <ctime>
#include <string>
#include <iostream>
    
using namespace std;

int main(int argc, char *argv[] ) {
    clock_t start, end;
    schema sc; 
    vector<instance> train_dataset, validate_dataset, test_dataset;
    readSchema(argv[1], sc);
    readInstances(argv[1], train_dataset, false);
    readInstances(argv[2], validate_dataset, false);
    readInstances(argv[3], test_dataset, true);
    //readSchema("btrain.csv", sc);
    //readInstances("btrain.csv", train_dataset, false);
    //readInstances("bvalidate.csv", validate_dataset, false);
    //readInstances("btest.csv", test_dataset, true);
    // generate a decision tree
    //start = clock();
    decisionTree dt(sc); 
    dt.train(train_dataset); 
    /*
    vector<int> countAttrNums;
    for (int j = -1; j < 10; j++) {
        countAttrNums = dt.countAttrNum(j);
        cout << "number of times being selected as splitting attr, " << "maxDepth = " << j << endl;
        cout << sc.attrNames[0] << ": " << countAttrNums[0];
        for (int i = 1; i < sc.attrNum; i++)
            cout << ", " << sc.attrNames[i] << ": " << countAttrNums[i];
        cout << endl;
    }
    end = clock();
    */
    //cout << "time to train the tree: " << double(end - start) / CLOCKS_PER_SEC << endl;
    cout << "correct rate on validation set: " << dt.validate(validate_dataset) << endl;
    cout << "correct rate on train set: " << dt.validate(train_dataset) << endl;
    //for (int i = 0; i < test_dataset.size(); i++) {
    //    test_dataset[i].flag = dt.predict(test_dataset[i]);
    //}
    //writeAll("testresult.csv", sc, test_dataset);
    return 0;
}

