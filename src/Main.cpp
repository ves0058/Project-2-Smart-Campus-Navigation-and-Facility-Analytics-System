#include "Manager.h"
#include "campusgraph.h"
#include "DepartmentTree.h"

int main()
{
    Manager manager;
    CampusGraph graph;
    DepartmentTree department; 

    string loadgraphbuildings = "data/buildings.txt";
    string loadgraphconnections = "data/connections.txt";
    string BuildingsFile = "data/buildings.txt";
    string FacilitiesFile = "data/facilities.txt";
	department.loadFromFile();
    graph.loadBuildings(loadgraphbuildings);
    graph.loadConnections(loadgraphconnections);
    manager.loadBuildingsFromFile(BuildingsFile);
    manager.loadFacilitiesFromFile(FacilitiesFile);
    
    manager.printHeader();

    string input;
    int choice;
    while (true)
    {
        manager.printMenu();
        cin >> input;
        cout << endl;

        if (manager.isChoiceCorrect(input))
        {
            choice = stoi(input); // converts string to int

            switch (choice)
            {
                case 1:
                    manager.viewFacilities();
                    break;
                case 2:
                    manager.searchFacility();
                    break;
                case 3:
                    graph.viewBuildingsandConnection();
                    break;
                case 4:
                    graph.generateGraphStats();
                    break;
                case 5:
                    graph.bfs();
                    break;
				case 6:
					department.addDepartment();
					break;
				case 7:
					department.searchDepartment();
					break;
				case 8:
					department.displayHierarchy();
					break;
				case 9:
					department.preorderTraversal();
					break;
                case 10:
                    cout << "Thank you for using \"Smart Campus Navigation and Facility Analytics System\"!" << endl;
                    return 0;
                default:
                    cout << "Wrong choice! try again." << endl;
                    cout << endl;
                    break;
            }
        }
        else
        {
            cout << "Wrong choice! try again." << endl;
            cout << endl;
        }
    }

    return 0;
}
