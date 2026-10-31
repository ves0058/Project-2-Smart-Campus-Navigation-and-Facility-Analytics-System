#include "campusgraph.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <queue>
using namespace std;

//File reading logic; 
//This will read the Buildings .h file and store it's data in names Vector
void CampusGraph::loadBuildings(const string& filename){
    ifstream file(filename);

        if(!file.is_open()){
            cout<<"Error :: File : "<<filename<<" can't be accessed "<<endl;
            return;
        }
    string line;
    int numberOfBuildings = 0;
    while(getline(file, line)){
        //Skipping # and empty line
        if(line.empty() || line[0] == '#'){
            continue;
        }
        string buildingName = line;
        numberOfBuildings++;
//load the building in to the name vector
        getOrAddNode(buildingName);
    }
    file.close();
}


//Creating Graphs connection logic
//Here we will read the connection data from the bonnections.h file
// and accourdlingy create those connections
void CampusGraph::loadConnections(const string& filename){
    ifstream file(filename);

    if(!file.is_open()){
        cout<<"Error :: File : "<<filename<<" can't be accessed "<<endl;
        return;
    }
    string line;
    while(getline(file, line)){
    stringstream ss(line);
    string buildingA, buildingB;

    //spiliting each line using '|' 
    //Here we are also making sure that before creating a building connection 
    // the two buildings are actually valid!
    if(getline(ss,buildingA,'|')&& getline(ss, buildingB)){
        bool validA = isAvalidBuilding(buildingA);
        bool validB = isAvalidBuilding(buildingB);
        if(validA && validB){
        addEdge(buildingA, buildingB);
        }
        else if( validA && !validB){
            cout<<"Building : "<<buildingB<<" don't exist in buildings list. Can't make the connection.!"<<endl;
        }
        else if(!validA && validB){
            cout<<"Building : "<<buildingA<<" don't exist in buildings list. Can't make the connection.!"<<endl;
        }
        else{
            cout<<"Building : "
                <<buildingA
                <<" and Building : "
                <<buildingB
                <<" Both buildings do not exist in the buildings list. Can't make the connection!"
                <<endl;
        }
    }
}
file.close();

}


/*Here we would either get the index of the building or add a new building to the names vectord
and keep up with the indexes for the integer vector that maps each building names with integer index.
*/

int CampusGraph::getOrAddNode(const string& name){
    for(int i =0; i<names.size(); i++){
        if(names[i] == name){
            return i;
        }
    }

    int newIdx = names.size();
    names.push_back(name);

    /*Creating an extra spot for the future building index
     so both names and adjList vectors can run paraller*/ 
    adjList.push_back({});

    return newIdx;
}


//This is heapler function to determin weather the buiding is a valid or not 
//cheacking the names vector
bool CampusGraph::isAvalidBuilding(const string& buildingName){
    bool isValid = false;
    for(int i=0; i<names.size(); i++){
        if(names[i] == buildingName){
            isValid = true;
        }
    }
    return isValid;
}

//BFS healper function; to make sure that the node actually exist in the graph
int CampusGraph::findNode(const string& nodeToFind){
    for(int i =0; i<names.size(); i++){
        if(names[i] == nodeToFind){
            return i;
        }
    }
    return -1;
}

//This is where we makes the connection for buildings(edges). undirected 
void CampusGraph::addEdge(const string& uNodeName, const string& vNodeName){
    int uNode = getOrAddNode(uNodeName);
    int vNode = getOrAddNode(vNodeName);

    adjList[uNode].push_back(vNode);
    adjList[vNode].push_back(uNode);
}

//

void CampusGraph::bfs(const string& startNode){
    int srcNode = findNode(startNode);
    if(srcNode == -1){
        cout<<"Node : "<<startNode<<" Don't exist in the current Graph. please try again!"<<endl;
        return;
    }

    vector<bool> status(names.size(), false);
    queue<int> que;

    que.push(srcNode);
    status[srcNode] = true;

    while (!que.empty()) {
            int u = que.front();
            que.pop();

            cout << names[u] << " ";

            for (int v : adjList[u]) {
                if (!status[v]) {
                    status[v] = true;
                    que.push(v);
                }
            }
        }
        cout << "\n";
}


