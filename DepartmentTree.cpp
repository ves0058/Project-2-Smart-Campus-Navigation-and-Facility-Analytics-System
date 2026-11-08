#include "DepartmentTree.h"

#include <fstream>
#include <sstream>
#include <stack>
#include <utility>

// This creates an empty department tree
DepartmentTree::DepartmentTree() {
    root = nullptr;
}

// This deletes the entire tree
DepartmentTree::~DepartmentTree() {
    if (root == nullptr) {
        return;
    }

    stack<DepartmentNode*> nodes;
    nodes.push(root);

    while (!nodes.empty()) {
        DepartmentNode* current = nodes.top();
        nodes.pop();

        for (size_t i = 0; i < current->children.size(); i++) {
            nodes.push(current->children[i]);
        }

        delete current;
    }
}

// Checks if the tree is empty
bool DepartmentTree::isEmpty() const {
    return root == nullptr;
}

// This function loads departments from the department file provided
void DepartmentTree::loadFromFile() {
    ifstream inputFile("data/departments.txt");

    if (!inputFile.is_open()) {
        cout << "Error opening departments.txt\n";
        return;
    }

    string line;
    int accepted = 0;
    int rejected = 0;
    int lineNumber = 0;

    while (getline(inputFile, line)) {
        lineNumber++;

        // Removes any whitespace from the beginning and end
        size_t first = line.find_first_not_of(" \t\r\n");

        if (first == string::npos) {
            continue;
        }

        size_t last = line.find_last_not_of(" \t\r\n");
        line = line.substr(first, last - first + 1);

        // Skips comments
        if (line[0] == '#') {
            continue;
        }

        string departmentName;
        string parentName;
        string extraField;

        stringstream record(line);

        getline(record, departmentName, '|');
        getline(record, parentName, '|');

        // Checkes for any extra fields
        if (getline(record, extraField, '|')) {
            cout << "Line " << lineNumber
                 << ": Incorrect number of fields.\n";
            rejected++;
            continue;
        }

        // Removes any whitespace from department name
        first = departmentName.find_first_not_of(" \t\r\n");

        if (first == string::npos) {
            cout << "Line " << lineNumber
                 << ": Empty department name.\n";
            rejected++;
            continue;
        }

        last = departmentName.find_last_not_of(" \t\r\n");
        departmentName =
            departmentName.substr(first, last - first + 1);

        // Removes any whitespace from parent node name
        first = parentName.find_first_not_of(" \t\r\n");

        if (first == string::npos) {
            cout << "Line " << lineNumber
                 << ": Empty parent name.\n";
            rejected++;
            continue;
        }

        last = parentName.find_last_not_of(" \t\r\n");
        parentName = parentName.substr(first, last - first + 1);

        // Check whether parent is NONE.
        string normalizedParent = parentName;

        for (size_t i = 0; i < normalizedParent.length(); i++) {
            normalizedParent[i] =
                tolower(static_cast<unsigned char>(
                    normalizedParent[i]));
        }

        // The first department becomes the root node
        if (root == nullptr) {
            if (normalizedParent != "none") {
                cout << "Line " << lineNumber
                     << ": First department must have NONE as parent.\n";
                rejected++;
                continue;
            }

            root = new DepartmentNode;
            root->name = departmentName;
            root->parent = nullptr;

            accepted++;
            continue;
        }

        // This rejectes another root node
        if (normalizedParent == "none") {
            cout << "Line " << lineNumber
                 << ": Additional root department.\n";
            rejected++;
            continue;
        }

        // This function searches for the department and its parent node
        DepartmentNode* parentNode = nullptr;
        bool duplicate = false;

        stack<DepartmentNode*> nodes;
        nodes.push(root);

        while (!nodes.empty()) {
            DepartmentNode* current = nodes.top();
            nodes.pop();

            string currentName = current->name;
            string compareName = departmentName;

            for (size_t i = 0; i < currentName.length(); i++) {
                currentName[i] =
                    tolower(static_cast<unsigned char>(
                        currentName[i]));
            }

            for (size_t i = 0; i < compareName.length(); i++) {
                compareName[i] =
                    tolower(static_cast<unsigned char>(
                        compareName[i]));
            }

            if (currentName == compareName) {
                duplicate = true;
            }

            string currentParent = current->name;
            string compareParent = parentName;

            for (size_t i = 0; i < currentParent.length(); i++) {
                currentParent[i] =
                    tolower(static_cast<unsigned char>(
                        currentParent[i]));
            }

            for (size_t i = 0; i < compareParent.length(); i++) {
                compareParent[i] =
                    tolower(static_cast<unsigned char>(
                        compareParent[i]));
            }

            if (currentParent == compareParent) {
                parentNode = current;
            }

            for (size_t i = 0; i < current->children.size(); i++) {
                nodes.push(current->children[i]);
            }
        }

        if (duplicate) {
            cout << "Line " << lineNumber
                 << ": Duplicate department.\n";
            rejected++;
            continue;
        }

        if (parentNode == nullptr) {
            cout << "Line " << lineNumber
                 << ": Parent department not found.\n";
            rejected++;
            continue;
        }

        // This creates and attaches the new department
        DepartmentNode* newNode = new DepartmentNode;

        newNode->name = departmentName;
        newNode->parent = parentNode;

        parentNode->children.push_back(newNode);

        accepted++;
    }

    inputFile.close();

    cout << "\nDepartment file loaded.\n";
    cout << "Accepted: " << accepted << "\n";
    cout << "Rejected: " << rejected << "\n";
}

// This function adds a department using user input
void DepartmentTree::addDepartment() {
    string departmentName;
    string parentName;

    cout << "Enter department name: ";
    getline(cin >> ws, departmentName);

    if (departmentName.empty()) {
        cout << "Department name cannot be empty.\n";
        return;
    }

    // This checks for any duplicate department names from input
    if (root != nullptr) {
        stack<DepartmentNode*> nodes;
        nodes.push(root);

        while (!nodes.empty()) {
            DepartmentNode* current = nodes.top();
            nodes.pop();

            string currentName = current->name;
            string compareName = departmentName;

            for (size_t i = 0; i < currentName.length(); i++) {
                currentName[i] =
                    tolower(static_cast<unsigned char>(
                        currentName[i]));
            }

            for (size_t i = 0; i < compareName.length(); i++) {
                compareName[i] =
                    tolower(static_cast<unsigned char>(
                        compareName[i]));
            }

            if (currentName == compareName) {
                cout << "Department already exists.\n";
                return;
            }

            for (size_t i = 0; i < current->children.size(); i++) {
                nodes.push(current->children[i]);
            }
        }
    }

    // This creates the root node if the tree is empty
    if (root == nullptr) {
        root = new DepartmentNode;

        root->name = departmentName;
        root->parent = nullptr;

        cout << "Root department added successfully.\n";
        return;
    }

    cout << "Enter parent department name: ";
    getline(cin >> ws, parentName);

    DepartmentNode* parentNode = nullptr;

    stack<DepartmentNode*> nodes;
    nodes.push(root);

    while (!nodes.empty()) {
        DepartmentNode* current = nodes.top();
        nodes.pop();

        string currentName = current->name;
        string compareName = parentName;

        for (size_t i = 0; i < currentName.length(); i++) {
            currentName[i] =
                tolower(static_cast<unsigned char>(
                    currentName[i]));
        }

        for (size_t i = 0; i < compareName.length(); i++) {
            compareName[i] =
                tolower(static_cast<unsigned char>(
                    compareName[i]));
        }

        if (currentName == compareName) {
            parentNode = current;
            break;
        }

        for (size_t i = 0; i < current->children.size(); i++) {
            nodes.push(current->children[i]);
        }
    }

    if (parentNode == nullptr) {
        cout << "Parent department not found.\n";
        return;
    }

    DepartmentNode* newNode = new DepartmentNode;

    newNode->name = departmentName;
    newNode->parent = parentNode;

    parentNode->children.push_back(newNode);

    cout << "Department added successfully.\n";
}

// This function searches for a department using user input
void DepartmentTree::searchDepartment() {
    if (root == nullptr) {
        cout << "No departments are loaded.\n";
        return;
    }

    string departmentName;

    cout << "Enter department to search for: ";
    getline(cin >> ws, departmentName);

    stack<DepartmentNode*> nodes;
    nodes.push(root);

    DepartmentNode* foundNode = nullptr;

    while (!nodes.empty()) {
        DepartmentNode* current = nodes.top();
        nodes.pop();

        string currentName = current->name;
        string compareName = departmentName;

        for (size_t i = 0; i < currentName.length(); i++) {
            currentName[i] =
                tolower(static_cast<unsigned char>(
                    currentName[i]));
        }

        for (size_t i = 0; i < compareName.length(); i++) {
            compareName[i] =
                tolower(static_cast<unsigned char>(
                    compareName[i]));
        }

        if (currentName == compareName) {
            foundNode = current;
            break;
        }

        for (size_t i = 0; i < current->children.size(); i++) {
            nodes.push(current->children[i]);
        }
    }

    if (foundNode == nullptr) {
        cout << "Department not found.\n";
        return;
    }

    int depth = 0;
    DepartmentNode* current = foundNode;

    while (current->parent != nullptr) {
        depth++;
        current = current->parent;
    }

    cout << "\nDepartment: " << foundNode->name << "\n";

    if (foundNode->parent == nullptr) {
        cout << "Parent: None\n";
    } else {
        cout << "Parent: " << foundNode->parent->name << "\n";
    }

    cout << "Direct children: "
         << foundNode->children.size() << "\n";

    cout << "Depth: " << depth << "\n";
}

// This displays the department hierarchy
void DepartmentTree::displayHierarchy() {
    if (root == nullptr) {
        cout << "No departments are loaded.\n";
        return;
    }

    stack<pair<DepartmentNode*, int>> nodes;

    nodes.push(make_pair(root, 0));

    while (!nodes.empty()) {
        pair<DepartmentNode*, int> current = nodes.top();
        nodes.pop();

        for (int i = 0; i < current.second; i++) {
            cout << "    ";
        }

        cout << current.first->name << "\n";

        // This pushes the child nodes in reverse order to preserve their order
        for (int i = current.first->children.size() - 1; i >= 0; i--) {
            nodes.push(make_pair(
                current.first->children[i],
                current.second + 1
            ));
        }
    }
}

// This fucntion displays departments using the preorder traversal
void DepartmentTree::preorderTraversal() {
    if (root == nullptr) {
        cout << "No departments are loaded.\n";
        return;
    }

    stack<DepartmentNode*> nodes;
    nodes.push(root);

    cout << "\nPreorder Traversal:\n";

    while (!nodes.empty()) {
        DepartmentNode* current = nodes.top();
        nodes.pop();

        cout << current->name << "\n";

        for (int i = current->children.size() - 1; i >= 0; i--) {
            nodes.push(current->children[i]);
        }
    }
}
