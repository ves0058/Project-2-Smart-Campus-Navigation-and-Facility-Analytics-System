#include <iostream>
#include "campusgraph.h"
using namespace std;

int main(){
    CampusGraph graph1;

    cout<<"Graph demo and check!\n";

cout << " 1 LOADING FILES " << endl;
    graph1.loadBuildings("data/buildings.txt");
    graph1.loadConnections("data/connections.txt");
    cout << endl;

    cout << "--- VIEW BUILDINGS AND CONNECTIONS ---" << endl;
    graph1.viewBuildingsandConnection();
    cout << endl;

    cout << "--- GRAPH STATS ---" << endl;
    graph1.generateGraphStats();
    cout << endl;

    cout << "--- TESTING BFS ---" << endl;
    graph1.bfs();

    return 0;

}

