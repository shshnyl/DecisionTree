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
    while (file_train.good() && count < 10) {
        instance inst(NUMATTRS);
        getline(file_train, line); istringstream inst_parser(line);
        for (int j = 0; j < NUMATTRS; j++) {
            getline(inst_parser, attr_val, ',');
            if (attr_val == "?") {
                inst.attrs[j].attrAvail = false;
                inst.attrs[j].attrVal = -1.0;
            } 
            else {
                inst.attrs[j].attrAvail = true;
                istringstream tmp(attr_val);
                tmp >> inst.attrs[j].attrVal;
                cout << inst.attrs[j].attrVal + 100 << endl;
            }
        }
        getline(inst_parser, attr_val, ',');
        cout << "flag: " << attr_val << endl;
        if (attr_val == "1") 
            inst.flag = true;
        else 
            inst.flag = false;
        count++;
    }

    file_train.close();
    return 0;
}

