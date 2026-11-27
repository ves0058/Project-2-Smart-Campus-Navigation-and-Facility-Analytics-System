#include "campusgraph.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <queue>
#include <cctype>
using namespace std;

//File reading logic; 
//This will read the Buildings .h file and store it's data in names Vector
//while also making sure of whitespace blanks lines....etc
void CampusGraph::loadBuildings(const string& filename){
    ifstream file(filename);

    if(!file.is_open()){
        cout << "Error :: File : " << filename << " can't be accessed " << endl;
        return;
    }

    string line;
    int lineNum = 0;
    int accepted = 0;
    int rejected = 0;

    while(getline(file, line)){
        lineNum++;

        // Trim whitespace and \r using a basic loop
        int start = 0;
        int end = line.size() - 1;
        while(start < (int)line.size() && (isspace(line[start]) || line[start] == '\r')){
            start++;
        }
        while(end >= start && (isspace(line[end]) || line[end] == '\r')){
            end--;
        }

        // Skip blank lines and comments..
        if(start > end || line[start] == '#'){
            continue;
        }

        string buildingName = "";
        for(int i = start; i <= end; i++){
            buildingName += line[i];
        }

        // Check if building already exists using normalName..
        bool duplicate = false;
        for(int i = 0; i < names.size(); i++){
            if(normalName(names[i]) == normalName(buildingName)){
                duplicate = true;
                break;
            }
        }

        if(duplicate){
            cout << "Warning: [" << filename << ":" << lineNum 
                 << "] Duplicate building: " << buildingName << endl;
            rejected++;
            continue;
        }

        // when everting is valid add the building to the vector! success!
        names.push_back(buildingName);
        adjList.push_back({});
        accepted++;
    }
    file.close();

    cout << filename << ": " << accepted << " accepted, " << rejected << " rejected" << endl;
}


//Creating Graphs connection logic
//Here we will read the connection data from the bonnections.h file
// and accourdlingy create those connections

void CampusGraph::loadConnections(const string& filename){
    ifstream file(filename);

    if(!file.is_open()){
        cout<<"Error :: "<< filename << " can't be accessed at the moment, please try again! "<<endl;
        return;
    }

    string line;
    int lineNum = 0;
    int accepted = 0;
    int rejected = 0;

    while(getline(file, line)){
        lineNum++;
        // trim whitespace and '\r' from the line
        int start = 0;
        int end = line.size() -1;// end of the line!

        while(start < (int)line.size() && (isspace(line[start]) || line[start] == '\r')){
            start++;
        }
        while(end >= start && (isspace(line[end]) || line[end] == '\r')){
            end --;
        }

        if(start > end || line[start] == '#'){
            continue;;
        }
        string clearLine = "";
        for(int i = start; i <= end; i++){
            clearLine += line[i];
        }

        stringstream ss(clearLine);

     string buildingA = "", buildingB = "", extra = "";

        // Reading building by building separated by '|' pipe and checking for any extra fields!
        if(!getline(ss, buildingA, '|') || !getline(ss, buildingB, '|') || getline(ss, extra)){
            cout << "Warning :: File : " << filename << " at line " << lineNum 
                 << " -> Malformed line: \"" << clearLine << "\". Skipping line!" << endl;
            rejected++;
            continue;
        }
        //Cleaning building A
        int buildingAstart = 0;
        int buildingAend = (int)buildingA.size() -1;

        //skipping leading spaces , tabls and '\r'
        while(buildingAstart < (int)buildingA.size()){
            if(buildingA[buildingAstart] == ' ' || buildingA[buildingAstart] == '\t' || buildingA[buildingAstart] == '\r'){
                buildingAstart++;
            }
            else{
                break;
            }
        }

        //skipping traling space tabs and '\r' at the end of the line
        while (buildingAend >= buildingAstart) {
            if (buildingA[buildingAend] == ' ' || buildingA[buildingAend] == '\t' || buildingA[buildingAend] == '\r') {
                buildingAend--;
            } else {
                break;
            }
        }
        // cleaned string building
        string cleanA = "";
        for (int i = buildingAstart; i <= buildingAend; i++) {
            cleanA += buildingA[i];
        }

        // Clean buildingB
        int bStart = 0;
        int bEnd = buildingB.size() - 1;

        // Skip leading spaces tabls and '\r'
        while (bStart < (int)buildingB.size()) {
            if (buildingB[bStart] == ' ' || buildingB[bStart] == '\t' || buildingB[bStart] == '\r') {
                bStart++;
            } else {
                break;
            }
        }

        // Skip leading spaces tabls and '\r' at the end
        while (bEnd >= bStart) {
            if (buildingB[bEnd] == ' ' || buildingB[bEnd] == '\t' || buildingB[bEnd] == '\r') {
                bEnd--;
            } else {
                break;
            }
        }

        // Build the cleaned string
        string cleanB = "";
        for (int i = bStart; i <= bEnd; i++) {
            cleanB += buildingB[i];
        }




        //////

        // Find index of both buildings
        int u = findNode(cleanA);
        int v = findNode(cleanB);

        // This is the crucial part of the code this is where we would 
        //  1. that not same connection are created twice,
        //  2. If a building don't exist in the building list, the connection can't be made
        //  3. if both buildings don't exist in the the building list then reject the load!

        if(u == -1 || v == -1){
            if(u == -1 && v != -1){
                cout << "Warning :: File : " << filename << " at line " << lineNum 
                     << " -> Building : " << cleanA << " does not exist in buildings list. Can't make connection!" << endl;
            } else if(u != -1 && v == -1){
                cout << "Warning :: File : " << filename << " at line " << lineNum 
                     << " -> Building : " << cleanB << " does not exist in buildings list. Can't make connection!" << endl;
            } else {
                cout << "Warning :: File : " << filename << " at line " << lineNum 
                     << " -> Both " << cleanA << " and " << cleanB << " do not exist in buildings list. Can't make connection!" << endl;
            }
            rejected++;
            continue;
        }

        // Rejecting self connection here! 
        if(u == v){
            cout << "Warning :: File : " << filename << " at line " << lineNum 
                 << " -> Building : " << cleanA << " cannot connect to itself. Can't make connection!" << endl;
            rejected++;
            continue;
        }

        // Rejecting Self loops and reverse conditions!
        bool alreadyConnected = false;
        for(int i = 0; i < adjList[u].size(); i++){
            if(adjList[u][i] == v){
                alreadyConnected = true;
                break;
            }
        }

        if(alreadyConnected){
            cout << "Warning :: File : " << filename << " at line " << lineNum 
                 << " -> Connection between " << cleanA << " and " << cleanB << " already exists. can't add duplicates!" << endl;
            rejected++;
            continue;
        }

        // If everting is good then adding a bidirectional connection
        adjList[u].push_back(v);
        adjList[v].push_back(u);
        accepted++;
    }
    file.close();
    //letting the user know how many buildings were accepted rejected! 
    cout << filename << ": " << accepted << " accepted, " << rejected << " rejected" << endl;

}



/*Here we would either get the index of the building or add a new building to the names vectord
and keep up with the indexes for the integer vector that maps each building names with integer index.
So basically the idead is to run the integer vector paraller to the buildings name vector
that way we could run the bfs and generate stats using integers index insted to strings...
*/

/*this is a healper function which would either make sure that
if the node alredy exist we don't create it again and if the node 
don't exist we can create it! */

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
//cheacking the names vector; invalid buildings will be disregarded!
bool CampusGraph::isAvalidBuilding(const string& buildingName){
    bool isValid = false;
    for(int i=0; i<names.size(); i++){
        if(names[i] == buildingName){
            isValid = true;
        }
    }
    return isValid;
}
// take the names input by the user and convers it to lowercase 
//to make every name standard when comparing! no mimatch between uppercase and lowercase
string CampusGraph::normalName(const string& name){
    string result = "";
      int start = 0;
    int end = name.size() - 1;

    // removes whitespace from the beginning
    while (start < (int)name.size() && isspace(name[start])) {
        start++;
    }

    // removes whitespace from the end
    while (end >= start && isspace(name[end])) {
        end--;
    }

    // converts remaining characters to lowercase..
    for (int i = start; i <= end; i++) {
        result += tolower(name[i]);
    }

    return result;
}



//BFS healper function; to make sure that the node actually exist in the graph
int CampusGraph::findNode(const string& nodeToFind) {
    for(int i = 0; i < names.size(); i++) {
        if(normalName(names[i]) == normalName(nodeToFind)) {
            return i;
        }
    }

    return -1;
}

//This is where we makes the connection for buildings(edges). undirected <->
void CampusGraph::addEdge(const string& uNodeName, const string& vNodeName){
    int uNode = getOrAddNode(uNodeName);
    int vNode = getOrAddNode(vNodeName);

    adjList[uNode].push_back(vNode);
    adjList[vNode].push_back(uNode);
}


// Running a basic bfs search with a given node by the user;
//We will also make sure that the input building name by the user is trimmed and good to use for check.
//will display bfs rechibility and distance as well!

void CampusGraph::bfs(){
    cout << "Enter starting building name: ";
    string input;
    cin.ignore();
    getline(cin, input);

    // Trim leading whitespace, tabs, and returns
    int start = 0;
    int end = input.size() - 1;
    while (start < (int)input.size() && (isspace(input[start]) || input[start] == '\r')) {
        start++;
    }
    while (end >= start && (isspace(input[end]) || input[end] == '\r')) {
        end--;
    }

    // Check if input was blank ____
    if (start > end) {
        cout << "!!!Error: Building name cannot be blank. please try again!!!" << endl;
        return;
    }

    string startNode = "";
    for (int i = start; i <= end; i++) {
        startNode += input[i];
    }

    int srcNode = findNode(startNode);//this uses the healper function to make sure that node exist in the names vector
    if(srcNode == -1){
        cout<<"!!!Node : "<<startNode<<" Does not exist in the current Graph. please try again!!!"<<endl;
        return;
    }

    vector<bool> status(names.size(), false);
    vector<int> distance(names.size(), -1);//to calculate the distance
    queue<int> que;//basice que implementation to run BFS

    cout<<"========== BFS REACHABILITY ==========\n";
    cout<<"++++++++++++++++++++++++++++++++++++++"<<endl;

    cout << "\nFrom " << names[srcNode] << ":" << endl;



    que.push(srcNode);
    status[srcNode] = true;
    distance[srcNode] = 0;

    while (!que.empty()) {
            int u = que.front();
            que.pop();
            //this is where it outputs the distance and name of the building
            cout << names[u] << " (distance " << distance[u] << ")" << endl;

            for (int v : adjList[u]) {
                if (!status[v]) {
                    status[v] = true;
                    distance[v] = distance[u] +1 ;
                    que.push(v);
                }
            }
        }
        cout << "\n\n";

        int reachableCounter = 0;
        for(auto i: status){
            if(i == true){
                reachableCounter++;
            }
        }
        //output the rechability count  and the actual size of the names vector as an comparision
        cout << "Reachable: " << reachableCounter << " of " << names.size() << " buildings" << endl;
}



//displaying the buildsing and connectinos
void CampusGraph::viewBuildingsandConnection()const{

    cout<<"========== CAMPUS BUILDINGS AND CONNECTIONS =========="<<endl;
    cout<<"++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;

    if(names.empty()){
        cout<<"This graph is curretly empty. please load graph data and try again!"<<endl;
        return;
    }

    //outputting the connections
    for (int i = 0; i < names.size(); i++)
    {

        cout << names[i] << endl;
        cout<<"\tConnections:"<<endl;

        if(adjList[i].empty()){
            cout<<"\t\t--> (no connections)"<<endl;
        }
        for (int neighbor : adjList[i])
        {
            cout << "\t\t-->"<<names[neighbor] << endl;
        }
    }
}

void CampusGraph::generateGraphStats()const{
    cout<<"========== CAMPUS GRAPH STATISTICS =========="<<endl;
    cout<<"+++++++++++++++++++++++++++++++++++++++++++++\n\n";

    cout<<"Total Buildings : "<< names.size()<<endl;

    int validConnection =0;

    for(int i =0; i < names.size(); i++){
            validConnection += adjList[i].size();
    }
    if(validConnection != 0){
        cout<<"Total Connections : "<< validConnection/2<<"\n\n\n";

    }
    else{
        cout<<"No valid connections in the graphs! \n\n\n";
    }
    

    cout<<"Buildings Degrees : \n\n";

    for(int i =0; i<names.size(); i++){
        cout<<names[i]<<" : "<<adjList[i].size()<<endl;
    }
    cout<<"\n\n";
}





//Final portion code!!!


void CampusGraph::shortestPath(){


    if(names.empty()){
        cout << "This graph is currently emty, please load graph data and try again!" << endl;
        return;
    }

    cout << "Enter starting building name --> ";
    string startInput;
    cin.ignore();
    getline(cin, startInput);

    // Trim leading and trailing whitespace for start building's name!
    //Same logic as what we have used in BFS function!
    int start = 0;
    int end = (int)startInput.size() - 1;
    while (start < (int)startInput.size() && (isspace(startInput[start]) || startInput[start] == '\r')) {
        start++;
    }
    while (end >= start && (isspace(startInput[end]) || startInput[end] == '\r')) {
        end--;
    }

    if (start > end) {
        cout << "Error: Building name cannot be blank. please try again!!!" << endl;
        return;
    }
    string startName = "";
    for (int i = start; i <= end; i++) {
        startName += startInput[i];
    }

    cout << "Enter destination building name --> ";
    string destInput;
    getline(cin, destInput);

    // Trim leading and trailing whitespace for destination building
    int destiStart = 0;
    int destiEnd = (int)destInput.size() - 1;
    while (destiStart < (int)destInput.size() && (isspace(destInput[destiStart]) || destInput[destiStart] == '\r')) {
        destiStart++;
    }
    while (destiEnd >= destiStart && (isspace(destInput[destiEnd]) || destInput[destiEnd] == '\r')) {
        destiEnd--;
    }

    if (destiStart > destiEnd) {
        cout << "Error: Building name cannot be blank. please try again!!" << endl;
        return;
    }

    string destName = "";
    for (int i = destiStart; i <= destiEnd; i++) {
        destName += destInput[i];
    }

    // Using findNode function to validate the given srcNode and destiNode!
    int srcNode = findNode(startName);
    int destiNode = findNode(destName);

    if (srcNode == -1 || destiNode == -1) {
        if (srcNode == -1 && destiNode != -1) {
            cout << "Node : " << startName << " Does not exist in the current Graph. please try again" << endl;
        } else if (srcNode != -1 && destiNode == -1) {
            cout << "Node : " << destName << " Does not exist in the current Graph. please try again" << endl;
        } else {
            cout << "Nodes : " << startName << " and " << destName << " Do not exist in the current Graph. please try again!" << endl;
        }
        return;
    }

    cout << "========== SHORTEST PATH {BFS} ==========\n";
    cout << "+++++++++++++++++++++++++++++++++++++++++" << endl;

    // Edge case will be  same start and destination building
    if (srcNode == destiNode) {
        cout << names[srcNode] << endl;
        cout << "Number of connections: 0 :(" << endl;
        return;
    }

    vector<bool> status(names.size(), false);//status vector to keep track of Visited and not visited
    vector<int> parent(names.size(), -1);
    queue<int> que;

    que.push(srcNode);
    status[srcNode] = true;

    while (!que.empty()) {
        int u = que.front();
        que.pop();

        if (u == destiNode) {
            break;
        }

        // Traverse to the closest negiboor first!
        for (size_t i = 0; i < adjList[u].size(); i++) {
            int v = adjList[u][i];

            if (!status[v]) {
                status[v] = true;
                parent[v] = u;
                que.push(v);
            }
        }
    }

    // If Destination Node could never be reached!
    if (!status[destiNode]) {
        cout << "No path exists between " << names[srcNode] << " and " << names[destiNode] << "." << endl;
        return;
    }

    // Re constructing path from destination back to start using parent vector
    vector<int> path;
    int curr = destiNode;
    while (curr != -1) {
        path.push_back(curr);
        curr = parent[curr];
    }

    // Print buildings in the order they were found!
    for (int i = (int)path.size() - 1; i >= 0; i--) {
        cout << names[path[i]] << endl;
    }
    cout << "Number of connections: " << (path.size() - 1) << endl;
}



// Option 17: Graph Connectivity Report 
void CampusGraph::connectivityReport()const {

    //if the Graph is empty! for some reason 
    if (names.empty()) {
        cout << "This graph is currently emty, please load graph data and try again!" << endl;
        return;
    }

    int totalBuildings = (int)names.size();
    int degreeSum = 0;
    int maxDegree = 0;
    vector<string> isolatedBuildings;

    // Calculate degree sum -- max degree --  and isolated buildings!!!!
    for (int i = 0; i < totalBuildings; i++) {
        int deg = (int)adjList[i].size();
        degreeSum += deg;

        if (deg == 0) {
            isolatedBuildings.push_back(names[i]);
        }

        if (deg > maxDegree) {
            maxDegree = deg;
        }
    }

    int totalConnections = degreeSum / 2;

    // Loop through all buildings and save every building that has the maximum number of connections
    // keeps them in the exact order they were loaded from the file, so we don't mess the data up
    vector<string> highestDegreeBuildings;
    for (int i = 0; i < totalBuildings; i++) {
        if ((int)adjList[i].size() == maxDegree) {
            highestDegreeBuildings.push_back(names[i]);
        }
    }

    // Count connected buildings using  BF Search, same logic we used in 

    vector<bool> status(names.size(), false);
    int connectedComponents = 0;

    for (int i = 0; i < totalBuildings; i++) {
        if (!status[i]) {
            connectedComponents++;
            queue<int> que;
            que.push(i);
            status[i] = true;

            while (!que.empty()) {
                int u = que.front();
                que.pop();

                for (size_t j = 0; j < adjList[u].size(); j++) {
                    int v = adjList[u][j];
                    if (!status[v]) {
                        status[v] = true;
                        que.push(v);
                    }
                }
            }
        }
    }

    //The final Graph COnnectivity Report !! for option 17
    cout << "========== GRAPH CONNECTIVITY REPORT ==========\n";
    cout << "+++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "Total buildings: " << totalBuildings << endl;
    cout << "Total connections: " << totalConnections << endl;
    //if the building is isolated!
    cout << "Isolated buildings: ";
    if (isolatedBuildings.empty()) {
        cout << "None" << endl;
    } else {
        for (size_t i = 0; i < isolatedBuildings.size(); i++) {
            cout << isolatedBuildings[i];
            if (i + 1 < isolatedBuildings.size()) {
                cout << ", ";
            }
        }
        cout << endl;
    }
    //Higest Degree
    cout << "Highest degree (" << maxDegree << "): ";
    for (size_t i = 0; i < highestDegreeBuildings.size(); i++) {
        cout << highestDegreeBuildings[i];
        if (i + 1 < highestDegreeBuildings.size()) {
            cout << ", ";
        }
    }
    cout << endl;
    //Good connected Buildings!
    cout << "Connected components: " << connectedComponents << endl;
}