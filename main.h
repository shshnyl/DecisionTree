#include "tree.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#define NUMATTRS 13
    
using namespace std;

void readSchema(string filename, schema &sc) {
    string line, attr_name;
    ifstream file(filename);
    if (!file.is_open()) 
        return ;
    sc.reserve(NUMATTRS);
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

void readInstances(string filename, vector<instance> &dataset, bool isTest) {
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
                inst_ptr->attrs[i].attrAvail = false;
                inst_ptr->attrs[i].attrVal = -9999.0;
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
        if (attr_val != "" && (isTest || attr_val != "?"))
            dataset.push_back(*inst_ptr);
        else 
            delete inst_ptr;
    }
    cout << dataset.size() << endl; 
    file.close();
}

void writeAll(string filename, schema &sc, vector<instance> &dataset) {
    ofstream file;
    file.open(filename);
    // schema
    for (int i = 0; i < NUMATTRS; i++) {
        file << sc.attrNames[i] << ",";
    }
    file << "winner" << endl;

    // instances
    for (int j = 0; j < dataset.size(); j++) {
        for (int i = 0; i < NUMATTRS; i++) {
            if (dataset[j].attrs[i].attrAvail)
                file << dataset[j].attrs[i].attrVal << ",";
            else
                file << "?,";
        }
        file << int(dataset[j].flag) << endl;
    }
    file.close();
}
