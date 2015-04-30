#include "tree.h"
#include <vector>
#include <iostream>

using namespace std;

int main() {
    int a = 9, b = 7;
    int data1[] = {123, 432, 343, 322, 323,533, 123, 422, 380};
    int data2[] = {482, 729, 7694, 839, 323, 877, 123};
    vector<instance *> ori1, ori2, des;
    for (int i = 0; i < a; i++) {
        instance *instPtr = new instance;
        instPtr->flag = false;
        instPtr->attrs[0].attrVal = data1[i];
        instPtr->attrs[0].attrAvail = true;
        ori1.push_back(instPtr);
    }
    for (int i = 0; i < b; i++) {
        instance *instPtr = new instance;
        instPtr->flag = false;
        instPtr->attrs[0].attrVal = data2[i];
        instPtr->attrs[0].attrAvail = true;
        ori2.push_back(instPtr);
    }
    sortInstances(0, ori1, ori2, des);
    cout << "ori: " << endl;
    cout << "des: " << endl; 
    for (int i = 0; i < a + b; i++) 
        cout << des[i]->attrs[0].attrVal << endl;;
    return 0;
}
