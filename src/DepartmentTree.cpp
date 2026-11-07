#include "DepartmentTree.h"

#include <cctype>
#include <fstream>
#include <sstream>

// Removes whitespace around a string.
static string trimText(const string& text) {
    size_t first = text.find_first_not_of(" \t\r\n");

    if (first == string::npos) {
        return "";
    }

    size_t last = text.find_last_not_of(" \t\r\n");

    return text.substr(first, last - first + 1);
}

// Converts a name to lowercase for comparison.
static string normalizeName(const string& text) {
    string result = trimText(text);

    for (size_t i = 0; i < result.length(); i++) {
        result[i] = static_cast<char>(
            tolower(static_cast<unsigned char>(result[i]))
        );
    }

    return result;
}

// Creates a department node.
DepartmentNode::DepartmentNode(string departmentName,
                               DepartmentNode* parentNode) {
    name = departmentName;
    parent = parentNode;
}

// Creates an empty tree.
DepartmentTree::DepartmentTree() {
    root = nullptr;
}

// Deletes the tree when the object is destroyed.
DepartmentTree::~DepartmentTree() {
    deleteTree(root);
}

// This returns true if the tree is empty.
bool DepartmentTree::isEmpty() const {
    return root == nullptr;
}

// This deletes all nodes recursively.
void DepartmentTree::deleteTree(DepartmentNode* node) {
    if (node == nullptr) {
        return;
    }

    for (size_t i = 0; i < node->children.size(); i++) {
        deleteTree(node->children[i]);
    }

    delete node;
}

// This function searches the tree recursively.
DepartmentNode* DepartmentTree::findNode(
    DepartmentNode* node, const string& name) const {

    if (node == nullptr) {
        return nullptr;
    }

    if (normalizeName(node->name) == normalizeName(name)) {
        return node;
    }

    for (size_t i = 0; i < node->children.size(); i++) {
        DepartmentNode* found =
            findNode(node->children[i], name);

        if (found != nullptr) {
            return found;
        }
    }

    return nullptr;
}

// This adds a department to the tree.
bool DepartmentTree::addDepartment(
    const string& name, const string& parentName) {

    string departmentName = trimText(name);
    string parent = trimText(parentName);

    if (departmentName.empty()) {
        cout << "Error: department name cannot be blank.\n";
        return false;
    }

    if (findNode(root, departmentName) != nullptr) {
        cout << "Error: department already exists: "
             << departmentName << "\n";
        return false;
    }

    // The first department becomes the root node.
    if (root == nullptr) {
        if (normalizeName(parent) != "none") {
            cout << "Error: root must use NONE as parent.\n";
            return false;
        }

        root = new DepartmentNode(departmentName);
        return true;
    }

    // Later departments need an existing parent node. 
    if (parent.empty() || normalizeName(parent) == "none") {
        cout << "Error: an existing parent is required.\n";
        return false;
    }

    DepartmentNode* parentNode = findNode(root, parent);

    if (parentNode == nullptr) {
        cout << "Error: parent department not found: "
             << parent << "\n";
        return false;
    }

    DepartmentNode* newNode =
        new DepartmentNode(departmentName, parentNode);

    parentNode->children.push_back(newNode);

    return true;
}

// This function loads and validates departments from the files.
void DepartmentTree::loadFromFile(const string& filename) {
    ifstream inputFile(filename);

    int accepted = 0;
    int rejected = 0;
    int lineNumber = 0;

    if (!inputFile.is_open()) {
        cout << "Error: could not open file "
             << filename << "\n";
        return;
    }

    string line;

    while (getline(inputFile, line)) {
        lineNumber++;

        line = trimText(line);

        if (line.empty() || line[0] == '#') {
            continue;
        }

        string departmentName;
        string parentName;
        string extraField;

        stringstream record(line);

        getline(record, departmentName, '|');
        getline(record, parentName, '|');

        departmentName = trimText(departmentName);
        parentName = trimText(parentName);

        // This checks for too many fields.
        if (getline(record, extraField, '|')) {
            cout << filename << ", line " << lineNumber
                 << ": wrong field count\n";
            rejected++;
            continue;
        }

        // This checks for any empty fields.
        if (departmentName.empty() || parentName.empty()) {
            cout << filename << ", line " << lineNumber
                 << ": empty field\n";
            rejected++;
            continue;
        }

        // The first valid record must be the root node
        if (root == nullptr &&
            normalizeName(parentName) != "none") {

            cout << filename << ", line " << lineNumber
                 << ": root must use NONE as parent\n";
            rejected++;
            continue;
        }

        // Reject another root node
        if (root != nullptr &&
            normalizeName(parentName) == "none") {

            cout << filename << ", line " << lineNumber
                 << ": additional root\n";
            rejected++;
            continue;
        }

        // Reject duplicate department names. (Inputs)
        if (findNode(root, departmentName) != nullptr) {
            cout << filename << ", line " << lineNumber
                 << ": duplicate department name\n";
            rejected++;
            continue;
        }

        // Reject unknown parents. (Inputs)
        if (root != nullptr &&
            findNode(root, parentName) == nullptr) {

            cout << filename << ", line " << lineNumber
                 << ": unknown parent: "
                 << parentName << "\n";
            rejected++;
            continue;
        }

        if (addDepartment(departmentName, parentName)) {
            accepted++;
        } else {
            rejected++;
        }
    }

    inputFile.close();

    cout << filename << ": " << accepted
         << " accepted, " << rejected << " rejected\n";
}

// This searches for a department and displays its information.
void DepartmentTree::searchDepartment(
    const string& name) const {

    DepartmentNode* node = findNode(root, name);

    if (node == nullptr) {
        cout << "Department not found: "
             << trimText(name) << "\n";
        return;
    }

    int depth = 0;
    DepartmentNode* current = node;

    while (current->parent != nullptr) {
        depth++;
        current = current->parent;
    }

    cout << "Department: " << node->name << "\n";

    if (node->parent == nullptr) {
        cout << "Parent: None\n";
    } else {
        cout << "Parent: " << node->parent->name << "\n";
    }

    cout << "Direct children: "
         << node->children.size() << "\n";

    cout << "Depth: " << depth << "\n";
}

// This displays the hierarchy with four spaces per depth.
void DepartmentTree::displayHierarchy(
    DepartmentNode* node, int depth) const {

    if (node == nullptr) {
        return;
    }

    for (int i = 0; i < depth; i++) {
        cout << "    ";
    }

    cout << node->name << "\n";

    for (size_t i = 0; i < node->children.size(); i++) {
        displayHierarchy(node->children[i], depth + 1);
    }
}

// Displays the complete hierarchy of the tree. 
void DepartmentTree::displayHierarchy() const {
    if (root == nullptr) {
        cout << "No departments are loaded.\n";
        return;
    }

    displayHierarchy(root, 0);
}

// This performs the preorder traversal.
void DepartmentTree::preorder(DepartmentNode* node) const {
    if (node == nullptr) {
        return;
    }

    cout << node->name << "\n";

    for (size_t i = 0; i < node->children.size(); i++) {
        preorder(node->children[i]);
    }
}

// This starts the preorder traversal.
void DepartmentTree::preorderTraversal() const {
    if (root == nullptr) {
        cout << "No departments are loaded.\n";
        return;
    }

    preorder(root);
}
