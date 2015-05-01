#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include "instance.h"
#define GAINRATIOTHRES 0.03

using namespace std;

class treeNode {
public:
    void addInstance(instance * inst, bool isPositive) { // add instance
        if (isPositive)
            this->posInstSet.push_back(inst);
        else
            this->negInstSet.push_back(inst);
    }

    int countNodes(schema &instSchema) {
        if (attrIdx == -1) {
            return 1;
        }
        else {
            if (instSchema.attrTypes[attrIdx]) // numeric data
                return 1 + this->left->countNodes(instSchema) + this->right->countNodes(instSchema);
            else { // nominal data
                int sum = 0;
                for (int i = 0; i < children.size(); i++)
                    sum += (children[i])->countNodes(instSchema);
                return 1 + sum;
            }
        }
    }

    int countLeafNodes(schema &instSchema) {
        if (attrIdx == -1) {
            return 1;
        }
        else {
            if (instSchema.attrTypes[attrIdx]) // numeric data
                return this->left->countLeafNodes(instSchema) + this->right->countLeafNodes(instSchema);
            else { // nominal data
                int sum = 0;
                for (int i = 0; i < children.size(); i++)
                    sum += (children[i])->countLeafNodes(instSchema);
                return sum;
            }
        }
    }

    vector<int> countAttrNum(schema &instSchema, int maxDepth) { // couting attr nums in the first maxDepth levels
        vector<int> result(instSchema.attrNum, 0);
        if (attrIdx == -1 || maxDepth == 0) { // if -1, means we do not limit the maxDepth
            return result;
        }
        else {
            if (instSchema.attrTypes[attrIdx]) { // numeric data
                vector<int> tmp = this->left->countAttrNum(instSchema, maxDepth - 1);
                for (int i = 0; i < instSchema.attrNum; i++)
                    result[i] += tmp[i];
                tmp = this->right->countAttrNum(instSchema, maxDepth - 1);
                for (int i = 0; i < instSchema.attrNum; i++)
                    result[i] += tmp[i];
                result[attrIdx]++;
                return result;
            }
            else { // nominal data
                vector<int> tmp;
                for (int i = 0; i < children.size(); i++) {
                    tmp = (children[i])->countAttrNum(instSchema, maxDepth - 1);
                    for (int j = 0; j < instSchema.attrNum; j++) 
                        result[j] += tmp[j];
                }
                result[attrIdx]++;
                return result;
            }
        }
    }

    void printDNF(schema &instSchema, string dnf) {
        string result;
        if (attrIdx == -1) { // leaf node
            cout << dnf << endl << endl;
        }
        else {
            if (instSchema.attrTypes[attrIdx]) { // numeric
                this->left->printDNF(instSchema, dnf + " AND " + "(" + instSchema.attrNames[attrIdx] + " < " + to_string(attrThres) + ")");
                this->right->printDNF(instSchema, dnf + " AND " + "(" + instSchema.attrNames[attrIdx] + " >= " + to_string(attrThres) + ")");
            }
            else { // nominal
                for (int i = 0; i < children.size(); i++) 
                    children[i]->printDNF(instSchema, dnf + " AND " + "(" + instSchema.attrNames[attrIdx] + " == " + to_string(attrLabels[attrIdx]) + ")");
            }
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
                    for (int i = 0; i < children.size(); i++)
                        (children[i])->split(instSchema);
                }
            }
        }
        return;
    }

    bool classify(schema &instSchema, instance &inst) {
        if (attrIdx == -1) {
            return (posInstSet.size() > negInstSet.size());
        }
        else {
            if (instSchema.attrTypes[attrIdx]) { // numeric
                if (inst.attrs[attrIdx].attrVal < attrThres) // left
                    return this->left->classify(instSchema, inst);
                else // right
                    return this->right->classify(instSchema, inst);
            }
            else { // nominal
                for (int i = 0; i < children.size(); i++) {
                    if (isEqual(inst.attrs[attrIdx].attrVal, attrLabels[i])) 
                        return children[i]->classify(instSchema, inst);
                }
                return false; // just in case of testing data we never met in training data
            }
        }
    }

private:
    int attrIdx = -1; // index the attrs to split 
    double attrThres = 0.0; // if numeric type, left < attrThres, right >= attrThres; if nominal type 
    vector<instance *> posInstSet; // set of positive instances TO BE split
    vector<instance *> negInstSet; // set of negative instances TO BE split
    treeNode * left = NULL, * right = NULL; // children ptrs when split over numeric attr
    vector<double> attrLabels; // children ptrs' label
    vector<treeNode *> children; // children ptrs when split over nominal attr
    
    bool trySplit(schema &instSchema) { // if the node need to be split
        if (posInstSet.size() == 0 || negInstSet.size() == 0) 
            return false;
        int maxIdx = -1;
        double maxThres = 0.0, maxGR = GAINRATIOTHRES;
        vector<double> maxLabels;
        for (int i = 0; i < instSchema.attrNum; i++) { // looking for the best attr to split over
            double threshold = 0.0, gainratio = 0.0;
            vector<double> labels;
            if (instSchema.attrTypes[i])
                this->trySplitNumeric(i, threshold, gainratio);
            else 
                this->trySplitNominal(i, labels, gainratio);

            if (gainratio > maxGR) {
                maxIdx = i;
                maxGR = gainratio;
                maxThres = threshold;
                maxLabels = labels;
            }
        }

        if (maxIdx == -1) // nothing greater than gainratio threshold
            return false;
        else {
            this->attrIdx = maxIdx;
            this->attrThres = maxThres;
            this->attrLabels = maxLabels;
            return true;
        }
    }

    void trySplitNumeric(int index, double &threshold, double &gr) { // split over numeric attr
        int numLeftP = 0, numLeftN = 0; // number of positive and negative ones in left subtree
        double maxThres = -1, maxGR = -1;
        // sort first
        vector<instance *> sortedInst;
        sortInstances(index, posInstSet, negInstSet, sortedInst);
        // traverse to find the optimal threshold
        maxThres = (sortedInst[0]->attrs[index]).attrVal;
        maxGR = this->calcGR(numLeftP, numLeftN);
        for (int i = 1; i < sortedInst.size(); i++) {
            // count
            if (sortedInst[i - 1]->flag) numLeftP++;
            else numLeftN++;
            // update max gain tratio
            double tmpThres = (sortedInst[i]->attrs[index]).attrVal;
            double tmpGR = this->calcGR(numLeftP, numLeftN); 
            if (tmpThres > maxThres) {
                if (tmpGR > maxGR) {
                    maxThres = tmpThres;
                    maxGR = tmpGR;
                }
            }
        }
        // return
        threshold = maxThres; 
        gr = maxGR;
    }

    void trySplitNominal(int index, vector<double> &labels, double &gr) { // split over nominal attr
        instance *tmpInst; attribute tmpAttr;
        vector<int> numP, numN;
        for (int i = 0; i < posInstSet.size(); i++) {
            tmpInst = posInstSet[i]; tmpAttr = tmpInst->attrs[index];
            int labelIdx = findInVector(labels, tmpAttr.attrVal);
            if (-1 == labelIdx) {
                labels.push_back(tmpAttr.attrVal);
                numP.push_back(0); numN.push_back(0);
                labelIdx = labels.size() - 1;
            }
            if (tmpInst->flag) 
                numP[labelIdx]++;
            else 
                numN[labelIdx]++;
        }
        for (int i = 0; i < negInstSet.size(); i++) {
            tmpInst = negInstSet[i]; tmpAttr = tmpInst->attrs[index];
            int labelIdx = findInVector(labels, tmpAttr.attrVal);
            if (-1 == labelIdx) {
                labels.push_back(tmpAttr.attrVal);
                numP.push_back(0); numN.push_back(0);
                labelIdx = labels.size() - 1;
            }
            if (tmpInst->flag) 
                numP[labelIdx]++;
            else 
                numN[labelIdx]++;
        }
        gr = this->calcGR(numP, numN);
    }

    int findInVector(vector<double> &labels, double key) {
        for (int i = 0; i < labels.size(); i++) {
            if(isEqual(labels[i], key))
                return i;
        }
        return -1;
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
            if (tmpAttr.attrVal < attrThres) // smaller than
                this->left->addInstance(tmpInst, true);
            else 
                this->right->addInstance(tmpInst, true);
        }
        for (int i = 0; i < negInstSet.size(); i++) {
            tmpInst = negInstSet[i];
            tmpAttr = (tmpInst->attrs)[attrIdx];
            if (tmpAttr.attrVal < attrThres) // smaller than
                this->left->addInstance(tmpInst, false);
            else 
                this->right->addInstance(tmpInst, false);
        }
        return true;
    }

    bool doSplitNominal() { // do the split over given attr on each value 
        if (attrIdx == -1) 
            return false;
        // do the split over attr with index attrIdx, and threshold attrThres
        for (int i = 0; i < attrLabels.size(); i++) {
            treeNode *tmp = new treeNode();
            children.push_back(tmp);
        }
        instance *tmpInst; attribute tmpAttr;
        for (int i = 0; i < posInstSet.size(); i++) {
            tmpInst = posInstSet[i];
            tmpAttr = (tmpInst->attrs)[attrIdx];
            for (int i = 0; i < children.size(); i++) {
                if (isEqual(tmpAttr.attrVal, attrLabels[i])) {
                    children[i]->addInstance(tmpInst, true);
                    break;
                }
            }
        }
        for (int i = 0; i < negInstSet.size(); i++) {
            tmpInst = negInstSet[i];
            tmpAttr = (tmpInst->attrs)[attrIdx];
            for (int i = 0; i < children.size(); i++) {
                if (isEqual(tmpAttr.attrVal, attrLabels[i])) {
                    children[i]->addInstance(tmpInst, false);
                    break;
                }
            }
        }
        return true;   
    }

    bool isEqual(double d1, double d2) {
        double threshold = 0.000001;
        return (d1 - d2 < threshold) && (d2 - d1 < threshold);
    }

    double calcGR(int leftP, int leftN) {
        int m = this->posInstSet.size(), n = negInstSet.size();
        int m1 = leftP, m2 = m - leftP, n1 = leftN, n2 = n - leftN;
        double p1 = double(m1 + n1) / double(m + n), p2 = double(m2 + n2) / double(m + n);
        return entropy(m, n) - p1 * entropy(m1, n1) - p2 * entropy(m2, n2);
    }

    double calcGR(vector<int> &numP, vector<int> &numN) {
        int len = numP.size(), m = this->posInstSet.size(), n = negInstSet.size();
        double result = entropy(m, n);
        for (int i = 0; i < len; i++) {
            double m1 = numP[i], n1 = numN[i];
            double p1 = double(m1 + n1) / double(m + n);
            result -= p1 * entropy(m1, n1);
        }
        return result;
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

    void printDNF() {
        this->root->printDNF(instSchema, "True");
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

    vector<int> countAttrNum(int maxDepth) {
        return this->root->countAttrNum(instSchema, maxDepth);
    }

    int countLeafNodes() {
        return this->root->countLeafNodes(instSchema);
    }
    
    int countNodes() {
        return this->root->countNodes(instSchema);
    }
};

