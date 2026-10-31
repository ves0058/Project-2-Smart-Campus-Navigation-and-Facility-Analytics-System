#ifndef CAMPUSGRAPH_H
#define CAMPUSGRAPH_H
#include <string>
#include <list>
#include <vector>
using namespace std;

class CampusGraph
{
    vector<string>names;
    vector<vector<int>>adjList;

    int getOrAddNode(const string& name);
    int findNode(const string& nodeToFind);
    void loadBuildings(const string& filename);
    void loadConnections(const string& filename);
    bool isAvalidBuilding(const string& buildingName);

    
    public:
    void addEdge(const string& uName, const string& vName);
    void bfs(const string& startNode);
};
#endif