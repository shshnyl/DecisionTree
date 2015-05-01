#include "tree.h"
#include <vector>
#include <ctime>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#define NUMATTRS 13
    
using namespace std;

void parseFile(string filename, schema &sc, vector<instance> &dataset);

int main() {
    clock_t start, end;
    vector<instance> dataset;
    schema sc(NUMATTRS); 
    parseFile("btrain.csv", sc, dataset);
    // generate a decision tree
    start = clock();
    decisionTree dt(sc);
    dt.train(dataset);
    dt.printTree();
    end = clock();
    cout << "time to train the tree: " << double(end - start) / CLOCKS_PER_SEC << endl;
    start = clock();
    cout << "correct rate: " << dt.validate(dataset) << endl;
    end = clock();
    cout << "time to validate: " << double(end - start) / CLOCKS_PER_SEC << endl;
    cout << "number of nodes: " << dt.countNodes() << endl; 
    return 0;
}

void parseFile(string filename, schema &sc, vector<instance> &dataset) {
    string line, attr_val;
    ifstream file_train(filename);
    if (!file_train.is_open()) 
        return ;
    // create schema from the first line
    bool attr_type[] = {true, true, false, true, true, true, false, false, true, true, false, true, true};
    //bool attr_type[] = {true, true, true, true, true, true, true, true, true, true, true, true, true};
    getline(file_train, line); istringstream sc_parser(line);
    for (int i = 0; i < NUMATTRS; i++) {
        getline(sc_parser, attr_val, ',');
        sc.attrNames[i] = attr_val;
        sc.attrTypes[i] = attr_type[i];
    }
    getline(sc_parser, attr_val, ','); // get rid of the "winner"
    // create instance line by line
    int count = 0; 
    while (file_train.good()) {
        instance *inst_ptr = new instance(NUMATTRS);
        getline(file_train, line); istringstream inst_parser(line);
        // attrs
        for (int i = 0; i < NUMATTRS; i++) {
            getline(inst_parser, attr_val, ',');
            if (attr_val == "?") {
                inst_ptr->attrs[i].attrAvail = true;//false;
                inst_ptr->attrs[i].attrVal = -1.0;
            } 
            else {
                inst_ptr->attrs[i].attrAvail = true;
                istringstream tmp(attr_val);
                tmp >> inst_ptr->attrs[i].attrVal;
            }
        }
        // flag
        getline(inst_parser, attr_val, ',');
        inst_ptr->flag = (attr_val == "1");
        // insert into our dataset
        if (attr_val != "" && attr_val != "?") 
            dataset.push_back(*inst_ptr);
        else {
            delete inst_ptr;
        }
    }
    cout << dataset.size() << endl << endl; 
    file_train.close();
}



