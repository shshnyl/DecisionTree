#include "tree.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#define NUMATTRS 13
    
using namespace std;

int main() {
    string filename = "btrain.csv";
    vector<instance> dataSet;
    ifstream fileTrain(filename);
    if (!fileTrain.is_open()) 
        return -1;
    // create schema from the first line
    schema sc(NUMATTRS); string attrname;
    //bool attrtype[] = {true, true, false, true, true, true, true, true, true, true, true, true, true, true};
    bool attrtype[] = {true, true, true, true, true, true, true, true, true, true, true, true, true};
    for (int i = 0; i < NUMATTRS; i++) {
        getline(fileTrain, attrname, ',');
        sc.attrNames[i] = attrname;
        sc.attrTypes[i] = attrtype[i];
    }
    getline(fileTrain, attrname, ','); // get rid of the "winner"
    cout << attrname << endl;

    // create instance line by line
    int count = 0; string attrvalue; 
    while (fileTrain.good() && count < 10) {
        count++;
        instance inst(NUMATTRS);
        for (int j = 0; j < NUMATTRS && fileTrain.good(); j++) {
            getline(fileTrain, attrvalue, ',');
            if (attrvalue == "?") {
                inst.attrs[j].attrAvail = false;
                inst.attrs[j].attrVal = -1.0;
                cout << attrvalue << endl;
            } 
            else {
                inst.attrs[j].attrAvail = true;
                istringstream tmp(attrvalue);
                tmp >> inst.attrs[j].attrVal;
                cout << inst.attrs[j].attrVal + 100000 << endl;
            }
        }
        if (!fileTrain.good()) break; // not enough for 
        getline(fileTrain, attrvalue, ',');
        cout << "flag: " << attrvalue << endl;
        if (attrvalue == "1") 
            inst.flag = true;
        else 
            inst.flag = false;
    }

    fileTrain.close();
    return 0;
}

