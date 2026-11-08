#ifndef DEPARTMENTTREE_H
#define DEPARTMENTTREE_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

// This structure represents one department.
struct DepartmentNode {
    string name;
    DepartmentNode* parent;
    vector<DepartmentNode*> children;

};

// This manages the department general tree.
class DepartmentTree {
private:
    DepartmentNode* root;

public:
    DepartmentTree();
    ~DepartmentTree();

    bool isEmpty() const;

    void loadFromFile();
    void addDepartment();
    void searchDepartment();
    void displayHierarchy();
    void preorderTraversal();
};

#endif

