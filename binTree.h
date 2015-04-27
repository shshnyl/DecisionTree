#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <priority_queue> 
#define NUMATTRS 14
#define GAINRATIOTHRES 0.1

using namespace std;

struct attribute { // attribute
    bool attrType; // true for numeric type, false for nominal
    bool attrAvail; // true for good data, false for missing data
    double attrVal; // value of the attribute

    attribute(bool type, bool avail, double value) {
        attrType = type;
        attrAvail = avail;
        attrVal = value;
    }
};

struct instance { // instance
    attribute attrs[NUMATTRS];
    bool flag;
};

struct schema { // schema for all the instances in a certain set(subtree)
    int attrNum = NUMATTRS;
    string attrNames[NUMATTRS]; // names for the attrs
    bool attrTypes[NUMATTRS]; // true for numeric ones, false for nominal ones
    double attrMin[NUMATTRS]; // this is constant for nominal ones
    double attrMax[NUMATTRS]; // this is constant for nominal ones
    schema(schema &oldSchema, int attrIndex, double attrThres, bool isLeft) {
        // copy
        for (int i = 0; i < NUMATTRS; i++) { // copy
            this->attrTypes[i] = oldSchema.attrTypes[i];
            this->attrMin[i] = oldSchema.attrMin[i];
            this->attrMax[i] = oldSchema.attrMax[i];
        }

        // update attrIndex
        if (this->attrTypes[attrIndex]) { // numeric attribute
            if (isLeft) { // schema for the left child
                this->attrMax[attrIndex] = attrThres;
            } 
            else { // shcema for the right child
                this->attrMin[attrIndex] = attrThres;
            }
        }
    }
};

class binTreeNode {
public:
    binTreeNode * left;
    binTreeNode * right;
    int attrIndex; // index the attrs to split 
    double attrThres; // if numeric type, left <= attrThres, right > attrThres; if nominal type, left == attrThres, right != attrThres
    schema instSchema; // schema of instances TO BE split
    vector<instance *> instSet; // set of instances TO BE split

    string toString() {
        string result = "the attribute is: ";
        result += this->instSchema.attrNames[this->attrIndex];
        result += ", the threshold is: ";
        result += this->attrThres;
        return result;
    }

    double split() {
        ;
    }

    double splitNumeric() { // split over numeric attr
        return 0.0;
    }

    double splitNominal() { // split over nominal attr
        return 0.0;
    }

    double entropy() { // by default, calculate a 
        return entropy(this->instSet);
    }

    static double entropy(vector<instance *> &set) {
        int positive = 0, negative = 0;
        for (int i = 0; i < set.size(); i++) {
            if ((set[i])->flag) // a positive instance 
                positive++;
            else 
                negative++;
        }
        return entropy(positive, negative);
    }

    static double entropy(int num1, int num2) { // entropy if we split in 2 groups
        if (num1 == 0) return 0;
        if (num2 == 0) return 0;
        double sum = num1 + num2;
        double p1 = num1 / sum, p2 = num2 / sum;
        return -(p1 * log(p1) + p2 * log(p2)) / log(2.0);
    }
};

class binTree {
public:
    binTreeNode * root;
    binTree(vector<instance> &instances) {
        this->root = this->initTree(instances);
    };
    binTreeNode * initTree(vector<instance> &instances);
};

binTreeNode * binTree::initTree(vector<instance> &instances) { // recursively generate the tree
    if (true) { // base cases
        ;
    }
    else {
        ;
    }
    return NULL;
}
