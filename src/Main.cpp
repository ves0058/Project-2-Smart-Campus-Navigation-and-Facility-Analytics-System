#include "Manager.h"


int main()
{
	Manager manager;

	manager.printHeader();

	string BuildingsFile = "data/buildings.txt";
	string FacilitiesFile = "data/facilities.txt";

	manager.loadBuildingsFromFile(BuildingsFile);
	manager.loadFacilitiesFromFile(FacilitiesFile);

	// FIXME: add your files and implementations...

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



		// ---------------------------FIXME:------------------------------------------			
				// case 3:
				// 	manager.cancelReservation();
				// 	break;
				// case 4:
				// 	manager.viewWaitingList();
				// 	break;
				// case 5:
				// 	manager.undoCancellation();
				// 	manager.availabilityManager();
				// 	break;
				// case 6:
				// 	manager.searchReservations();
				// 	break;
				// case 7:
				// 	manager.sortResources();
				// 	break;
				// case 8:
				// 	manager.generateReport();
				// 	break;
				// case 9:
				// 	manager.viewCancellationHistory();
				// 	break;
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
