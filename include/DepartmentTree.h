#ifndef DEPARTMENTTREE_H
#define DEPARTMENTTREE_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;
// This structure represents one department.
struct DepartmentNode {
    string name; // Stores the department names
    DepartmentNode* parent; // Points to the parent department
    vector<DepartmentNode*> children; // Stores the child departments
};
// This class manages the department general tree.
class DepartmentTree {
private:
    DepartmentNode* root; // This points to the first department
public:
    DepartmentTree();
    ~DepartmentTree();

    bool isEmpty() const; // This checks if the tree is empty 

    void loadFromFile(); // Loads departments from the files provided
    void addDepartment(); // Adds a new department
    void searchDepartment(); // Searches for a department
    void displayHierarchy(); // Displays the department hierarchy
    void preorderTraversal(); // Displays the departments using preorder traversal
};

#endif

