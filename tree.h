#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include "instance.h"
#define GAINRATIOTHRES 0.02

using namespace std;

class treeNode {
public:
    void addInstance(instance * inst, bool isPositive) { // add instance
        if (isPositive)
            this->posInstSet.push_back(inst);
        else
            this->negInstSet.push_back(inst);
    }

    int countNodes() {
        if (attrIdx == -1) {
            return 1;
        }
        else {
            return this->left->countNodes() + this->right->countNodes();
        }
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

    bool classify(schema &instSchema, instance &inst) {
        if (attrIdx == -1) {
            return (posInstSet.size() >= negInstSet.size());
        }
        else {
            if (instSchema.attrTypes[attrIdx]) { // numeric
                if (inst.attrs[attrIdx].attrVal < attrThres) { // left
                    return this->left->classify(instSchema, inst);
                }
                else { // right
                    return this->right->classify(instSchema, inst);
                }
            }
            else { // nominal
                // to be implemented
                return false;
            }
        }
        return false;
    }

    void printSubTree(schema &instSchema) {
        if (attrIdx == -1) {
            cout << "leaf node! Entropy: " << entropy() << ", pos: " << posInstSet.size() << ", neg: " << negInstSet.size() << endl;
        }
        else {
            cout << "attr is: " << attrIdx << endl;
            cout << "threshold is: " << this->attrThres << endl << endl;
            this->left->printSubTree(instSchema);
            this->right->printSubTree(instSchema);
        }
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

            //cout << "curr gr: " << gainratio << endl;
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
        
        /*
        cout << "sorted: ";
        for (int i = 0; i < sortedInst.size(); i++) 
            cout << sortedInst[i]->attrs[index].attrVal << ", ";
        cout << endl; 
        */
        // traverse to find the optimal threshold
        maxThres = (sortedInst[0]->attrs[index]).attrVal;
        maxGR = this->calcGR(numLeftP, numLeftN);
        for (int i = 1; i < sortedInst.size(); i++) {
            // count
            if (sortedInst[i - 1]->flag) numLeftP++;
            else numLeftN++;

            double tmpThres = (sortedInst[i]->attrs[index]).attrVal;
            double tmpGR = this->calcGR(numLeftP, numLeftN); 
            /*
            if (tmpThres > maxThres) {
            cout << tmpThres << ", " << tmpGR << endl;
            cout << numLeftP << ", " << numLeftN << endl;
            cout << entropy() << ", "; 
            cout << entropy(numLeftP, numLeftN) << ", ";
            cout << entropy(posInstSet.size() - numLeftP, negInstSet.size() - numLeftN) << endl;
            }
            */
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
        int m1 = leftP, m2 = m - leftP, n1 = leftN, n2 = n - leftN;
        double p1 = double(m1 + n1) / double(m + n), p2 = double(m2 + n2) / double(m + n);
        return entropy(m, n) - p1 * entropy(m1, n1) - p2 * entropy(m2, n2);
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
    decisionTree(schema &dataSchema) {
        this->instSchema.copy(dataSchema);
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
        cout << "training is over!" << endl << endl;
        return true;
    }

    bool predict(instance &inst) { // predict a single inst, return its predicted flag
        return this->root->classify(instSchema, inst);
    }

    double validate(vector<instance> &instances) { // return the correct rate
        int rightCount = 0, wrongCount = 0;
        for (int i = 0; i < instances.size(); i++) {
            instance tmpInst = instances[i];
            if (tmpInst.flag == this->predict(tmpInst)) // hit!!!
                rightCount++;
            else // miss :-(
                wrongCount++;
        }
        return double(rightCount) / double(rightCount + wrongCount);
    }

    int countNodes() {
        return this->root->countNodes();
    }
};

