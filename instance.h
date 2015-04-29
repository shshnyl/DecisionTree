#include <string>
#define NUMATTRS 14

using namespace std;

struct attribute { // attribute
    bool attrAvail; // true for good data, false for missing data
    double attrVal; // value of the attribute
};

struct instance { // instance
    attribute attrs[NUMATTRS];
    bool flag;
};

class schema { // schema for all the instances in a certain set(subtree)
public: 
    int attrNum = NUMATTRS;
    string attrNames[NUMATTRS]; // names for the attrs
    bool attrTypes[NUMATTRS]; // true for numeric ones, false for nominal ones
};

