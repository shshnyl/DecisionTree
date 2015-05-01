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
    //readSchema(argv[1], sc);
    //readInstances(argv[1], train_dataset, false);
    //readInstances(argv[2], validate_dataset, false);
    //readInstances(argv[3], test_dataset, true);
    readSchema("btrain.csv", sc);
    readInstances("btrain.csv", train_dataset, false);
    readInstances("bvalidate.csv", validate_dataset, false);
    readInstances("btest.csv", test_dataset, true);
    // building learning curve
    /*
    int times = 10;
    int seed[] = {349, 345, 499, 8273, 10342, 343, 3242, 9389, 2332, 12332};
    double sum = 0.0;
    for (double p = 0.1; p <= 0.9; p += 0.1) {
        sum = 0;
        for (int i = 0; i < times; i++) {
            decisionTree dt(sc);
            dt.train(train_dataset, seed[i], p);
            sum += dt.validate(validate_dataset); 
        }
        cout << "final averge is: " << sum / double(times) << " when p is: " << p << endl;
    }
    */
    // generate a decision tree
    //start = clock();
    decisionTree dt(sc); 
    dt.train(train_dataset); 
    //dt.printDNF();
    //dt.countAttrNum(3);
    //dt.countAttrNum(5);
    //end = clock();
    //cout << "time to train the tree: " << double(end - start) / CLOCKS_PER_SEC << endl;
    cout << "correct rate on validation set: " << dt.validate(validate_dataset) << endl;
    cout << "correct rate on train set: " << dt.validate(train_dataset) << endl;
    //write to file
    //for (int i = 0; i < test_dataset.size(); i++) {
    //    test_dataset[i].flag = dt.predict(test_dataset[i]);
    //}
    //writeAll("testresult.csv", sc, test_dataset);
    return 0;
}

