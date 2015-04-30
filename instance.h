#include <string>
#include <queue>
#include <vector>
#include <iostream>

using namespace std;

struct attribute { // attribute
    bool attrAvail; // true for good data, false for missing data
    double attrVal; // value of the attribute
};

class instance { // instance
public:
    vector<attribute> attrs;
    bool flag;

    instance(int num) {
        this->attrs.resize(num);
        this->flag = true;
    }

    instance(int num, bool f) {
        this->attrs.resize(num);
        this->flag = f;
    }
};

class schema { // schema for all the instances in a certain set(subtree)
public:
    int attrNum;
    vector<string> attrNames; // names for the attrs
    vector<bool> attrTypes; // true for numeric ones, false for nominal ones

    schema() {;}

    schema(int num) {
        this->attrNum = num;
        this->attrNames.resize(num);
        this->attrTypes.resize(num);
    }

    void copy(schema &sc) {
        this->attrNum = sc.attrNum;
        this->attrNames = sc.attrNames;
        this->attrTypes = sc.attrTypes;
    }
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
}
