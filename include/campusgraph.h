#ifndef CAMPUSGRAPH_H
#define CAMPUSGRAPH_H
#include <string>
#include <vector>
using namespace std;

class CampusGraph
{
    vector<string>names;//this will store the buildings name!
    vector<vector<int>>adjList;//this will run praller to the names vector and will map each string to a integer

    int getOrAddNode(const string& name);//checks if a node already exist in the graph or not?
    int findNode(const string& nodeToFind);
    bool isAvalidBuilding(const string& buildingName);//check if the building is valid or not?
    string normalName(const string& name);// converts inputed name to lowercse so camparision can be done!

    
    public:
    void loadBuildings(const string& filename);//load building from the data file
    void loadConnections(const string& filename);//loads connections from the data file
    void addEdge(const string& uName, const string& vName);//creates edges between buildings
    void bfs();//runs bfs search and tells the building rechibility
    void viewBuildingsandConnection()const;// outputs every building and it's connections
    void generateGraphStats()const;// outputs graph stats
    string inputFromUser(const string& input);//get's the input from user to run a BFS search
    
};
#endif