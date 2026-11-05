#include <iostream>
#include "campusgraph.h"
using namespace std;


//This is just a temp file that i've created just to see if my logic works!
//Remove it when we will acutally make the main file!

int main(){
    CampusGraph graph1;

    cout<<"Graph demo and check!\n";

cout << " Load files " << endl;
    graph1.loadBuildings("data/buildings.txt");
    graph1.loadConnections("data/connections.txt");
    cout << endl;

    cout << "display campus graph" << endl;
    graph1.viewBuildingsandConnection();
    cout << endl;

    cout << "graph stats" << endl;
    graph1.generateGraphStats();
    cout << endl;

    cout << "-Bfs test" << endl;
    graph1.bfs();

    return 0;

}

