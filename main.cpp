#include "tree.h"
#include <vector>
#include <iostream>

using namespace std;

int main() {
    int a = 9, b = 7;
    int data1[] = {123, 432, 343, 422, 323,533, 123, 422, 380};
    int data2[] = {482, 729, 769, 839, 323, 877, 123};
    vector<instance *> ori1, ori2, des;
    vector<instance> data;
    for (int i = 0; i < a; i++) {
        instance *instPtr = new instance;
        instPtr->flag = (i % 2 == 0);
        instPtr->attrs[0].attrVal = data1[i];
        instPtr->attrs[0].attrAvail = true;
        ori1.push_back(instPtr);
        data.push_back(*instPtr);
    }
    for (int i = 0; i < b; i++) {
        instance *instPtr = new instance;
        instPtr->flag = (i % 2 == 0);
        instPtr->attrs[0].attrVal = data2[i];
        instPtr->attrs[0].attrAvail = true;
        ori2.push_back(instPtr);
        data.push_back(*instPtr);
    }
    sortInstances(0, ori1, ori2, des);

    for (int i = 0; i < a + b; i++) 
        cout << "sorted result: " << des[i]->attrs[0].attrVal << ", " << des[i]->flag << endl;
    cout << endl;

//    for (int i = 0; i < a + b; i++) 
//        cout << "whole data: " << data[i].attrs[0].attrVal << ", " << data[i].flag << endl;


    schema sc; sc.attrTypes[0] = true; sc.attrNames[0] = "test col";
    decisionTree dt(sc);

    dt.train(data);
    dt.printTree();
    return 0;

}
