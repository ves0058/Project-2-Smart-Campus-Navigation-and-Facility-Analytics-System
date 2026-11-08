# Project 2: Smart Campus Navigation and Facility Analytics System
The Smart Campus Navigation and Facility Analytics System is a program that is designed to help students explore campus facilities, building connections, and find academic department information. The program loads campus information from files, organizes the information using different data structures, and provides a menu for accessing the available features.

The project runs using three main data structures: a custom hash table for facility ID searches, an undirected graph for campus building connections, and a general tree for the academic department hierarchy.

**Features**

1. View all facilities -

2. Search facility by ID -

3. View campus graph -

4. Display graph counts and degrees - 

5. Find reachable buildings (BFS) -

6. Add department - 

7. Search department -

8. Display department hierarchy -

9. Run preorder traversal - 

**Data Structures Used**

1. Facility ID Hash Table 
2. Undirected Campus Graph 
3. Department Tree 

**Files**

data: 
  buildings.txt
  connections.txt
  departments.txt
  facilities.txt
include:
  DepartmentTree.h
  Facility.h
  HashTable.h
  Manager.h
  campusgraph.h
src:
  DepartmentTree.cpp
  Facility.cpp
  HashTable.cpp
  Main.cpp
  Manager.cpp
  campusgraph.cpp
testdata:
  .gitkeep
  buildings.txt
  facilities.txt
MakeFile
README.txt
CONTRIBUTIONS.txt
design_document.txt
test_results.txt
.gitignore

To compile, write: 
1) make
2) ./project2Milestone.exe
