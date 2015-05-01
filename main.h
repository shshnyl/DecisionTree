#include "tree.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#define NUMATTRS 13
    
using namespace std;

void parseSchema(string filename, schema &sc) {
    string line, attr_name;
    ifstream file(filename);
    if (!file.is_open()) 
        return ;
    // create schema from the first line
    bool attr_type[] = {true, true, false, true, true, true, false, false, true, true, false, true, true};
    //bool attr_type[] = {true, true, true, true, true, true, true, true, true, true, true, true, true};
    getline(file, line); istringstream sc_parser(line);
    for (int i = 0; i < NUMATTRS; i++) {
        getline(sc_parser, attr_name, ',');
        sc.attrNames[i] = attr_name;
        sc.attrTypes[i] = attr_type[i];
    }
    file.close();
}

void parseInstances(string filename, vector<instance> &dataset) {
    int count = 0;
    string line, attr_val;
    ifstream file(filename);   
    // create instance line by line
    if (!file.is_open()) 
        return;
    getline(file, line);
    while (file.good()) {
        instance *inst_ptr = new instance(NUMATTRS);
        getline(file, line); istringstream inst_parser(line);
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
    file.close();
}

