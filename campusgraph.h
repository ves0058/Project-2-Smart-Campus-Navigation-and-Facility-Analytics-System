#ifndef CAMPUSGRAPH_H
#define CAMPUSGRAPH_H
#include <string>
#include <vector>
using namespace std;

class CampusGraph
{
    vector<string>names;
    vector<vector<int>>adjList;

    int getOrAddNode(const string& name);
    int findNode(const string& nodeToFind);
    bool isAvalidBuilding(const string& buildingName);
    string normalName(const string& name);

    
    public:
    void loadBuildings(const string& filename);
    void loadConnections(const string& filename);
    void addEdge(const string& uName, const string& vName);
    void bfs();
    void viewBuildingsandConnection()const;
    void generateGraphStats()const;
    string inputFromUser(const string& input);
    
};
#endif