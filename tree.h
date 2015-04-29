#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <queue> 
#include "instance.h"
#define GAINRATIOTHRES 0.1

using namespace std;

class treeNode {
private:
    vector<treeNode *> children; // children ptrs when split over nominal attr
    treeNode * left = NULL, * right = NULL; // children ptrs when split over numeric attr
    int attrIdx = -1; // index the attrs to split 
    double attrThres = 0.0; // if numeric type, left <= attrThres, right > attrThres; if nominal type 
    schema *instSchema = NULL; // schema of instances
    vector<instance *> posInstSet; // set of positive instances TO BE split
    vector<instance *> negInstSet; // set of negative instances TO BE split

public:
    treeNode(schema *oriSchema) { // empty instance sets
        this->instSchema = oriSchema;
    }

    treeNode(schema *oriSchema, vector<instance *> instSet1, vector<instance *> instSet2) { // given instance sets
        this->instSchema = oriSchema;
        this->posInstSet = instSet1; 
        this->negInstSet = instSet2;
    }

    void addInstance(instance * inst, bool isPositive) { // add instance
        if (isPositive)
            this->posInstSet.push_back(inst);
        else
            this->negInstSet.push_back(inst);
    }

    bool trySplit() { // if the node need to be split
        // if true, split it and update the attrIndex;
        // otherwise, set attrIndex to be -1 
        int maxIdx = -1;
        double maxThres = 0.0;
        double maxGR = GAINRATIOTHRES;
        for (int i = 0; i < instSchema->attrNum; i++) { // looking for the best attr to split over
            double threshold, gainratio;
            if (instSchema->attrTypes[i])
                this->trySplitNumeric(i, &threshold, &gainratio);
            else 
                this->trySplitNominal(i, &threshold, &gainratio);
            if (gainratio > maxGR) {
                maxIdx = i;
                maxThres = threshold;
                maxGR = gainratio;
            }
        }
        if (maxIdx == -1) // nothing greater than gainratio threshold
            return false;
        else {
            this->attrIdx = maxIdx;
            this->attrThres = maxThres;
            return true;
        }
    }

    bool doSplit() {
        if (instSchema->)
        ;
    }

    bool doSplitNumberic() { // do the split over given attr and threshold
        if (attrIdx == -1) 
            return false;
        // do the split over attr with index attrIdx, and threshold attrThres
        this->left = new treeNode(instSchema);
        this->right = new treeNode(instSchema);
        instance *tmpInst; attribute tmpAttr;
        for (int i = 0; i < posInstSet.size(); i++) {
            tmpInst = posInstSet[i];
            tmpAttr = (tmpInst->attrs)[attrIdx];
            if (tmpAttr.attrAvail) { // normal data
                if (tmpAttr.attrVal <= attrThres) // numeric and no greater than
                    this->left->addInstance(tmpInst, true);
                else 
                    this->rigth->addInstance(tmpInst, true);
            }
            else { // bad data
                ; // to be implemented
            }
        }
        for (int i = 0; i < negInstSet.size(); i++) {
            tmpInst = negInstSet[i];
            tmpAttr = (tmpInst->attrs)[attrIdx];
            if (tmpAttr.attrAvail) { // normal data
                if (tmpAttr.attrVal <= attrThres) // numeric and no greater than
                    this->left->addInstance(tmpInst, false);
                else 
                    this->right->addInstance(tmpInst, false);
            }
            else { // bad data
                ; // to be implemented
            }
        }
        return true;
    }

    bool doSplitNominal() { // do the split over given attr on each value 
         if (attrIdx == -1) 
            return false;
        // do the split over attr with index attrIdx, and threshold attrThres
        instance *tmpInst; attribute tmpAttr;
        for (int i = 0; i < posInstSet.size(); i++) {
            tmpInst = posInstSet[i];
            tmpAttr = (tmpInst->attrs)[attrIdx];
            if (tmpAttr.attrAvail) { // normal data
            }
            else { // bad data
                ; // to be implemented
            }
        }
        for (int i = 0; i < negInstSet.size(); i++) {
            tmpInst = negInstSet[i];
            tmpAttr = (tmpInst->attrs)[attrIdx];
            if (tmpAttr.attrAvail) { // normal data
            }
            else { // bad data
                ; // to be implemented
            }
        }
        return true;   
    }

    void trySplitNumeric(int index, double *threshold, double *gr) { // split over numeric attr
        // sort over the attribute first
        // then traverse all the instances
    }

    void trySplitNominal(int index, double *threshold, double *gr) { // split over nominal attr
        // to be impletmented
    }

    double entropy() { // by default, calculate the entropy before splitting  
        return entropy(this->posInstSet.size(), this->negInstSet.size());
    }

    static double entropy(int num1, int num2) { // entropy if we split in 2 groups
        if (num1 == 0) return 0;
        if (num2 == 0) return 0;
        double sum = num1 + num2;
        double p1 = num1 / sum, p2 = num2 / sum;
        return -(p1 * log(p1) + p2 * log(p2)) / log(2.0);
    }

    string toString() { // to string
        // leaf node
        if (attrIdx == -1)
            return "leaf node";
        // internal node
        string result = "";
        result += "the attribute is: ";
        result += instSchema->attrNames[attrIdx];
        result += ", the type is: ";
        if (instSchema->attrTypes[attrIdx]) 
            result += "Numerical";
        else
            result += "Nominal";
        result += ", the threshold is: ";
        result += this->attrThres;
        return result;
    }

};

class decisionTree {
private:
    treeNode * root = NULL; // default an empty tree
    vector<instance> instSet; // all the training data
    schema instSchema;
public:
    decisionTree(schema dataSchema, vector<instance> &dataSet) {
        this->instSchema = dataSchema;
        this->instSet = dataSet;
        this->root = this->initTree(dataSet);
    };

    treeNode * initTree(vector<instance> &instances);
    bool classifyInst() {
        return false;
    }
};

treeNode * decisionTree::initTree(vector<instance> &instances) { // recursively generate the tree
    if (true) { // base cases
        ;
    }
    else {
        ;
    }
    return NULL;
}
