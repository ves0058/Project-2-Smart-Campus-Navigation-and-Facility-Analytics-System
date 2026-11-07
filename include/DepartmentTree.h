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

    DepartmentNode(string departmentName,
                   DepartmentNode* parentNode = nullptr);
};

// This manages the department general tree.
class DepartmentTree {
private:
    DepartmentNode* root;

    DepartmentNode* findNode(DepartmentNode* node,
                             const string& name) const;

    void displayHierarchy(DepartmentNode* node,
                          int depth) const;

    void preorder(DepartmentNode* node) const;

    void deleteTree(DepartmentNode* node);

public:
    DepartmentTree();
    ~DepartmentTree();

    DepartmentTree(const DepartmentTree&) = delete;
    DepartmentTree& operator=(const DepartmentTree&) = delete;

    bool isEmpty() const;

    bool addDepartment(const string& name,
                       const string& parentName);

    void searchDepartment(const string& name) const;

    void displayHierarchy() const;

    void preorderTraversal() const;
};

#endif

