#include <string>
#include <queue>
#include <vector>
#include <iostream>
#define NUMATTRS 1

using namespace std;

struct attribute { // attribute
    bool attrAvail; // true for good data, false for missing data
    double attrVal; // value of the attribute
};

struct instance { // instance
    attribute attrs[NUMATTRS];
    bool flag;
};

struct schema { // schema for all the instances in a certain set(subtree)
    int attrNum = NUMATTRS;
    string attrNames[NUMATTRS]; // names for the attrs
    bool attrTypes[NUMATTRS]; // true for numeric ones, false for nominal ones
};

class instComparator {
public:
    int idx = -1;
    instComparator(int idx) {
        this->idx = idx;
    }

    bool operator() (const instance *a, const instance *b) {
        return !((a->attrs[idx]).attrVal <= (b->attrs[idx]).attrVal);
    }
};

void sortInstances(int attrIdx, vector<instance *> &ori, vector<instance *> &des) {
    int n = ori.size(); des.resize(n);
    instComparator comp(attrIdx);
    priority_queue<instance *, vector<instance *>, instComparator> Q(comp);
    for (int i = 0; i < n; i++)
        Q.push(ori[i]);
    for (int i = 0; i < n; i++) {
        des[i] = Q.top();
        Q.pop();
    }
}

void mergeSortedInstances(int attrIdx, vector<instance *> &ori1, vector<instance *> &ori2, vector<instance *> &des) {
    int m = ori1.size(), n = ori2.size(); des.resize(m + n);
}

void sortInstances(int attrIdx, vector<instance *> &ori1, vector<instance *> ori2, vector<instance *> &des) {
    vector<instance *> ori;
    ori.reserve(ori1.size() + ori2.size());
    ori.insert(ori.end(), ori1.begin(), ori1.end());
    ori.insert(ori.end(), ori2.begin(), ori2.end());
    sortInstances(attrIdx, ori, des);
    //sortInstances(attrIdx, ori1, sorted1);
    //sortInstances(attrIdx, ori2, sorted2);
    //mergeSortedInstances(attrIdx, sorted1, sorted2, des);
}
