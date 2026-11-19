#include "DepartmentTree.h"

#include <fstream>
#include <sstream>
#include <stack>
#include <utility>
#include <limits>

// This constructor starts the tree as empty
DepartmentTree::DepartmentTree() {
    root = nullptr;
}
// This destructor will delete all nodes in the tree
DepartmentTree::~DepartmentTree() {
    if (root == nullptr) {
        return;
    }
    stack<DepartmentNode*> nodes; // Implemented a stack to visit and delete every node
    nodes.push(root);

    while (!nodes.empty()) {
        DepartmentNode* current = nodes.top();
        nodes.pop();

        for (size_t i = 0; i < current->children.size(); i++) {  // Add children to the stack before it deletes the current node
            nodes.push(current->children[i]);
        }
        delete current;
    }
}
bool DepartmentTree::isEmpty() const {  // Will check if the tree is empty
    return root == nullptr;
}
// This function loads departments from the department file provided
void DepartmentTree::loadFromFile() {
    ifstream inputFile("data/departments.txt"); // Opens the department file and checks if opened successfully

    if (!inputFile.is_open()) {
        cout << "Error opening departments.txt\n";
        return;
    }
    const string displayName = "departments.txt";

    string line;
    int accepted = 0;
    int rejected = 0;
    int lineNumber = 0;

    while (getline(inputFile, line)) {  // This will read the file one line at a time
        lineNumber++;
        size_t first = line.find_first_not_of(" \t\r\n");

        if (first == string::npos) { // Skip the blank lines
            continue;
        }
        size_t last = line.find_last_not_of(" \t\r\n");
        line = line.substr(first, last - first + 1);

        if (line[0] == '#') {  // Skip the comment lines in the file
            cout << "File name: \"" << displayName << "\", Line number: " << lineNumber
                 << ", Reason: comment line." << endl;
            continue;
        }
        string departmentName;
        string parentName;
        string extraField;

        stringstream record(line); // This will seperate the department name and the parent name

        getline(record, departmentName, '|');
        getline(record, parentName, '|');

        if (getline(record, extraField, '|')) { // Rejects any lines that contain too many fields
            cout << "File name: \"" << displayName << "\", Line number: " << lineNumber
                 << ", Reason: Incorrect number of fields." << endl;
            rejected++;
            continue;
        }
        first = departmentName.find_first_not_of(" \t\r\n"); // Removes extra spaces from the parent name

        if (first == string::npos) {
            cout << "File name: \"" << displayName << "\", Line number: " << lineNumber
                 << ", Reason: Empty department name." << endl;
            rejected++;
            continue;
        }
        last = departmentName.find_last_not_of(" \t\r\n");
        departmentName =
            departmentName.substr(first, last - first + 1);

        first = parentName.find_first_not_of(" \t\r\n"); // Remove extra spaces from the parent name 

        if (first == string::npos) { 
            cout << "File name: \"" << displayName << "\", Line number: " << lineNumber
                 << ", Reason: Empty parent name." << endl;
            rejected++;
            continue;
        }
        last = parentName.find_last_not_of(" \t\r\n");
        parentName = parentName.substr(first, last - first + 1);

        string normalizedParent = parentName; // Converts the parent name to lowercase for comparing

        for (size_t i = 0; i < normalizedParent.length(); i++) { 
            normalizedParent[i] =
                tolower(static_cast<unsigned char>(
                    normalizedParent[i]));
        }
        if (root == nullptr) { // This ensures that the first department must become the root
            if (normalizedParent != "none") {
                cout << "File name: \"" << displayName << "\", Line number: " << lineNumber
                     << ", Reason: First department must have NONE as parent." << endl;
                rejected++;
                continue;
            }
            root = new DepartmentNode;
            root->name = departmentName;
            root->parent = nullptr;

            accepted++;
            continue;
        }
        if (normalizedParent == "none") { // This will reject another department trying to become the root
            cout << "File name: \"" << displayName << "\", Line number: " << lineNumber
                 << ", Reason: Additional root department." << endl;
            rejected++;
            continue;
        }

        // This function searches for the department and its parent node along with checking for any duplicates
        DepartmentNode* parentNode = nullptr;
        bool duplicate = false;

        stack<DepartmentNode*> nodes;
        nodes.push(root);
        
        while (!nodes.empty()) {
            DepartmentNode* current = nodes.top();
            nodes.pop();

            // This will compare the department names without having to worry about capitalization
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
            // Checks whether the current node is the requested parent 
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
            // This visits the current node's children
            for (size_t i = 0; i < current->children.size(); i++) {
                nodes.push(current->children[i]);
            }
        }
        // Rejects any duplicate department names
        if (duplicate) {
            cout << "File name: \"" << displayName << "\", Line number: " << lineNumber
                 << ", Reason: Duplicate department." << endl;
            rejected++;
            continue;
        }
        // Rejects any departments whose parent does not exist in the tree 
        if (parentNode == nullptr) {
            cout << "File name: \"" << displayName << "\", Line number: " << lineNumber
                 << ", Reason: Parent department not found." << endl;
            rejected++;
            continue;
        }
        // This creates and attaches the new department to its parent 
        DepartmentNode* newNode = new DepartmentNode;

        newNode->name = departmentName;
        newNode->parent = parentNode;

        parentNode->children.push_back(newNode);

        accepted++;
    }
    inputFile.close();

    // This will display how many departments were accepted and rejected
    cout << displayName << ": " << accepted << " accepted, " << rejected << " rejected" << endl;
    cout<<endl;
}
// This function adds a department using user input
void DepartmentTree::addDepartment() {
    string departmentName;
    string parentName;

   cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter department name: ";
    getline(cin, departmentName);

    if (departmentName.empty()) { // Does not allow any empty department name 
        cout << "Department name cannot be empty.\n";
        return;
    } 
    if (root != nullptr) { // Checks if the department already exists
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
            for (size_t i = 0; i < current->children.size(); i++) { // Adds the children to the stack to continue searching 
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
    // This will ask for the parent department 
    cout << "Enter parent department name: ";
    getline(cin >> ws, parentName);

    DepartmentNode* parentNode = nullptr;

    stack<DepartmentNode*> nodes; // Searches for the parent department
    nodes.push(root);

    while (!nodes.empty()) {
        DepartmentNode* current = nodes.top();
        nodes.pop();

        string currentName = current->name;
        string compareName = parentName;

        for (size_t i = 0; i < currentName.length(); i++) { // Compares the names without any capitalization differences 
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
        for (size_t i = 0; i < current->children.size(); i++) { // Continues searching through the children in the tree
            nodes.push(current->children[i]);
        }
    }
    if (parentNode == nullptr) { // Does not add the department if its parent isn't found in the tree
        cout << "Parent department not found.\n";
        return;
    }
    DepartmentNode* newNode = new DepartmentNode; // Creates the new department and connects it to its parent 

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

    stack<DepartmentNode*> nodes; // Uses a stack to search through the tree nodes
    nodes.push(root);

    DepartmentNode* foundNode = nullptr;

    while (!nodes.empty()) {
        DepartmentNode* current = nodes.top();
        nodes.pop();

        string currentName = current->name;
        string compareName = departmentName;

        for (size_t i = 0; i < currentName.length(); i++) { // Converts the names to lowercase for comparing 
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

        for (size_t i = 0; i < current->children.size(); i++) { // Adds child nodes to continue the search 
            nodes.push(current->children[i]);
        }
    }
    if (foundNode == nullptr) { // Displays this if the department isn't found 
        cout << "Department not found.\n";
        return;
    }
    int depth = 0; // Calculates the departments depth by following its parents
    DepartmentNode* current = foundNode;

    while (current->parent != nullptr) {
        depth++;
        current = current->parent;
    }
    cout << "\nDepartment: " << foundNode->name << "\n"; // Displays the departments information

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
    stack<pair<DepartmentNode*, int> > nodes;

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
    stack<DepartmentNode*> nodes; // Starts the traversal at the root
    nodes.push(root);

    cout << "\nPreorder Traversal:\n";

    while (!nodes.empty()) {
        DepartmentNode* current = nodes.top();
        nodes.pop();

        cout << current->name << "\n"; // Visits the parent before the child

        for (int i = current->children.size() - 1; i >= 0; i--) { // Pushes the children in reverse order to visit them in the original order
            nodes.push(current->children[i]);
        }
    }
}
