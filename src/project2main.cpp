#include <iostream>
#include "campusgraph.h"
using namespace std;

int main(){
    CampusGraph graph1;

  

    //loading graph files
    graph1.loadBuildings("data/buildings.txt");
    graph1.loadConnections("data/connections.txt");
    graph1.viewBuildingsandConnection();
    graph1.generateGraphStats();
    graph1.bfs();
    return 0;

}

