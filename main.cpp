#include "tree.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#define NUMATTRS 13
    
using namespace std;

int main() {
    string filename = "btrain.csv", line, attr_val;
    vector<instance> dataSet;
    ifstream file_train(filename);
    if (!file_train.is_open()) 
        return -1;

    // create schema from the first line
    schema sc(NUMATTRS); 
    bool attrtype[] = {true, true, true, true, true, true, true, true, true, true, true, true, true};
    getline(file_train, line); istringstream sc_parser(line);
    for (int i = 0; i < NUMATTRS; i++) {
        getline(sc_parser, attr_val, ',');
        sc.attrNames[i] = attr_val;
        sc.attrTypes[i] = attrtype[i];
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
        if (attr_val != "") 
            dataSet.push_back(*inst_ptr);
        else {
            delete inst_ptr;
        }
    }
    cout << dataSet.size() << endl << endl; 
    file_train.close();

    // generate a decision tree
    decisionTree dt(sc);
    dt.train(dataSet);

    //dt.printTree();

    return 0;
}

