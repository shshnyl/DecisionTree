#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include "instance.h"
#define GAINRATIOTHRES 0.01

using namespace std;

class treeNode {
public:
    void addInstance(instance * inst, bool isPositive) { // add instance
        if (isPositive)
            this->posInstSet.push_back(inst);
        else
            this->negInstSet.push_back(inst);
    }

    void split(schema &instSchema) { // split through the sub-tree
        if (trySplit(instSchema)) { // can split
            if (doSplit(instSchema)) { // successfully split the data
                if (instSchema.attrTypes[attrIdx]) { // numeric
                    this->left->split(instSchema);
                    this->right->split(instSchema);
                }
                else { // nominal
                    for (int i = 0; i < this->children.size(); i++)
                        (this->children[i])->split(instSchema);
                }
            }
        }
        return;
    }

    void printSubTree(schema &instSchema) {
        cout << endl;
        cout << this->toString(instSchema) << endl;
        if (attrIdx != -1) {
        cout << "threshold is: " << this->attrThres << endl;
        this->left->printSubTree(instSchema);
        this->right->printSubTree(instSchema);
        }
        else {
            cout << "positive ones: ";
            for (int i = 0; i < posInstSet.size(); i++) 
                cout << posInstSet[i]->attrs[0].attrVal << " ";
            cout << endl;
            cout << "negative ones: ";
            for (int i = 0; i < negInstSet.size(); i++) 
                cout << negInstSet[i]->attrs[0].attrVal << " ";
            cout << endl;
        }
    }

    string toString(schema &instSchema) { // to string
        // leaf node
        if (attrIdx == -1)
            return "leaf node";
        // internal node
        string result = "";
        result += "the attribute is: ";
        result += instSchema.attrNames[attrIdx];
        result += ", the type is: ";
        if (instSchema.attrTypes[attrIdx]) 
            result += "Numerical";
        else
            result += "Nominal";
        //result += ", the threshold is: ";
        //result += toString(this->attrThres);
        return result;
    }

private:
    int attrIdx = -1; // index the attrs to split 
    double attrThres = 0.0; // if numeric type, left < attrThres, right >= attrThres; if nominal type 
    vector<instance *> posInstSet; // set of positive instances TO BE split
    vector<instance *> negInstSet; // set of negative instances TO BE split
    treeNode * left = NULL, * right = NULL; // children ptrs when split over numeric attr
    vector<treeNode *> children; // children ptrs when split over nominal attr
    
    bool trySplit(schema &instSchema) { // if the node need to be split
        if (posInstSet.size() == 0 || negInstSet.size() == 0) 
            return false;
        int maxIdx = -1;
        double maxThres = 0.0;
        double maxGR = GAINRATIOTHRES;
        for (int i = 0; i < instSchema.attrNum; i++) { // looking for the best attr to split over
            double threshold, gainratio;
            if (instSchema.attrTypes[i])
                this->trySplitNumeric(i, &threshold, &gainratio);
            else 
                this->trySplitNominal(i, &threshold, &gainratio);
            cout << "curr gain ratio is: " << gainratio << endl;

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

    void trySplitNumeric(int index, double *threshold, double *gr) { // split over numeric attr
        // sort over the attribute first
        // then traverse all the instances
        int numLeftP = 0, numLeftN = 0; // number of positive and negative ones in left subtree
        double maxThres = -1, maxGR = -1;
        // sort
        vector<instance *> sortedInst;
        sortInstances(index, posInstSet, negInstSet, sortedInst);
        
        cout << "sorted: ";
        for (int i = 0; i < sortedInst.size(); i++) 
            cout << sortedInst[i]->attrs[index].attrVal << ", ";
        cout << endl; 
        
        // traverse to find the optimal threshold
        maxThres = (sortedInst[0]->attrs[index]).attrVal;
        maxGR = this->calcGR(numLeftP, numLeftN);
        for (int i = 1; i < sortedInst.size(); i++) {
            // count
            if (sortedInst[i - 1]->flag) numLeftP++;
            else numLeftN++;

            double tmpThres = (sortedInst[i]->attrs[index]).attrVal;
            double tmpGR = this->calcGR(numLeftP, numLeftN); 
            if (tmpThres > maxThres) {
            cout << tmpThres << ", " << tmpGR << endl;
            cout << numLeftP << ", " << numLeftN << endl;
            cout << entropy() << ", "; 
            cout << entropy(numLeftP, numLeftN) << ", ";
            cout << entropy(posInstSet.size() - numLeftP, negInstSet.size() - numLeftN) << endl;
            }
            if (tmpThres > maxThres) {
                if (tmpGR > maxGR) {
                    maxThres = tmpThres;
                    maxGR = tmpGR;
                }
            }
        }
        // return
        *threshold = maxThres; 
        *gr = maxGR;
    }

    void trySplitNominal(int index, double *threshold, double *gr) { // split over nominal attr
        // to be impletmented
        *threshold = 0.0;
        *gr = -1;
    }

    bool doSplit(schema &instSchema) {
        if (instSchema.attrTypes[attrIdx])  // numeric ones
            return this->doSplitNumberic(); 
        else // nominal ones
            return this->doSplitNominal();
    }

    bool doSplitNumberic() { // do the split over given attr and threshold
        if (attrIdx == -1) 
            return false;
        // do the split over attr with index attrIdx, and threshold attrThres
        this->left = new treeNode();
        this->right = new treeNode();
        instance *tmpInst; attribute tmpAttr;
        for (int i = 0; i < posInstSet.size(); i++) {
            tmpInst = posInstSet[i];
            tmpAttr = (tmpInst->attrs)[attrIdx];
            if (tmpAttr.attrAvail) { // normal data
                if (tmpAttr.attrVal < attrThres) // numeric and no greater than
                    this->left->addInstance(tmpInst, true);
                else 
                    this->right->addInstance(tmpInst, true);
            }
            else { // bad data
                ; // to be implemented
            }
        }
        for (int i = 0; i < negInstSet.size(); i++) {
            tmpInst = negInstSet[i];
            tmpAttr = (tmpInst->attrs)[attrIdx];
            if (tmpAttr.attrAvail) { // normal data
                if (tmpAttr.attrVal < attrThres) // numeric and no greater than
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

    double calcGR(int leftP, int leftN) {
        int m = this->posInstSet.size(), n = negInstSet.size();
        return entropy(m, n) - entropy(leftP, leftN) - entropy(m - leftP, n - leftN);
    }

    double entropy() { // by default, calculate the entropy before splitting  
        return entropy(this->posInstSet.size(), this->negInstSet.size());
    }

    double entropy(int num1, int num2) { // entropy if we split in 2 groups
        if (num1 == 0) return 0;
        if (num2 == 0) return 0;
        double sum = num1 + num2;
        double p1 = num1 / sum, p2 = num2 / sum;
        return -(p1 * log(p1) + p2 * log(p2)) / log(2.0);
    }
};

class decisionTree {
private:
    treeNode * root = NULL; // default an empty tree
    schema instSchema;
public:
    decisionTree(schema dataSchema) {
        this->instSchema = dataSchema;
        this->root = NULL; 
    };

    void printTree() {
        this->root->printSubTree(instSchema);
    }

    bool train(vector<instance> &instances) {
        cout << "start to train the tree!" << endl;
        this->root = new treeNode();
        cout << "adding instances!" << endl;
        for (int i = 0; i < instances.size(); i++) // init a new treeNode with our instance
            root->addInstance(&(instances[i]), instances[i].flag);
        cout << "splitting!" << endl;
        root->split(instSchema);
        cout << "training is over!" << endl;
        return true;
    }

    bool classify(instance inst) {
        return false;
    }
};

