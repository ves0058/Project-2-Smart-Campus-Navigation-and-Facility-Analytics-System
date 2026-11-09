#include "Manager.h"


void Manager::printHeader() // banner
{
	cout<<"+------------------------------------------------------+"<<endl;
	cout<<"|         Computer Science and Engineering             |"<<endl;
	cout<<"|    CSCE 2110 - Foundations of Data Structures        |"<<endl;
	cout<<"|     Rodion  rd0824   RodionDvoretskii@my.unt.edu     |"<<endl;
    cout<<"|     Arumit  ak2624   ArumitKumar@my.unt.edu          |"<<endl;
    cout<<"|     Vic     ves0058  VicScorgie@my.unt.edu           |"<<endl;
	cout<<"+------------------------------------------------------+"<<endl;
    cout << endl;
}


void Manager::printMenu() // prints the menu of all possible options
{
	cout << "====== Smart Campus Navigation and Facility Analytics System ======" << endl;
    cout << "1.  View all facilities" << endl;
    cout << "2.  Search facility by ID" << endl;
    cout << "3.  View campus graph" << endl;
    cout << "4.  Display graph counts and degrees" << endl;
    cout << "5.  Find reachable buildings (BFS)" << endl;
    cout << "6.  Add department" << endl;
    cout << "7.  Search department" << endl;
    cout << "8.  Display department hierarchy" << endl;
    cout << "9.  Run preorder traversal" << endl;
    cout << "10. Exit" << endl;
    cout << endl;
    cout<<"Enter Choice: ";
}


// validation
bool Manager::isChoiceCorrect(string input) // choice vailadtion
{
	if (input.empty())
	{
		return false;
	}

	char c;

	for (int i = 0; i < (int) input.length(); i++)
	{
		c = input[i];

		if (!isdigit(c))
		{
			return false;
		}
	}

	return true;
}


bool Manager::doesBuildingNameExist(string BuildingName)
{
	vector<string>:: iterator iter;
	for (iter = buildings.begin(); iter != buildings.end(); iter++)
	{
		if (equalsIgnoreCase(*iter, BuildingName))
		{
			return true;
		}
	}

	return false;
}


string Manager::trim(string line)
{
	int start = 0;
	int end = line.size() - 1;

	while (start <= end && isspace(line[start]))
	{
		start++;
	}

	while (end >= start && isspace(line[end]))
	{
		end--;
	}

	return line.substr(start, end - start + 1);
}


bool Manager::equalsIgnoreCase(string s1, string s2)
{
	if (s1.size() != s2.size())
	{
		return false;
	}

	for (int i = 0; i < (int) s1.size(); i++)
	{
		char a1 = s1[i];
		char a2 = s2[i];
		
		if (tolower(a1) != tolower(a2))
		{
			return false;
		}
	}

	return true;
}


bool Manager::isBuildingNameUnique(string BuildingName)
{
	vector<string>::iterator iter;
	for (iter = buildings.begin(); iter != buildings.end(); iter++)
	{
		if (equalsIgnoreCase(*iter, BuildingName))
		{
			return false;
		}
	}

	return true;
}


bool Manager::isFacilityIDValid(string &FacilityID)
{
	if (FacilityID.empty())
	{
		return false;
	}

	char s1 = FacilityID[0];
	char s2 = FacilityID[1];
	char s3 = FacilityID[2];
	char s4 = FacilityID[3];

	if (FacilityID.length() == 4 && (toupper(s1) == 'F') && isdigit(s2) && isdigit(s3) && isdigit(s4))
	{
		return true;
	}

	return false;
}


bool Manager::isBuildingNameValid(string BuildingName)
{
	char s;
	for (int i = 0; i < (int) BuildingName.size(); i++)
	{
		s = BuildingName[i];
		if (!(isalpha(s) || isdigit(s) || isspace(s)))
		{
			return false;
		}
	}

	return true;
}


void Manager::loadBuildingsFromFile(string fileName)
{
	ifstream fin;
	fin.open(fileName);
	
	// validates file-opening
	if (fin.fail())
	{
		cout << "File error: cannot open. Treated as empty." << endl;
		cout << "buildings.txt: 0 accepted, 0 rejected" << endl;
		return;
	}

	int accepted = 0;
	int rejected = 0;

	int lineNumber = 1;

	string buildingName;
	while (getline(fin, buildingName))
	{
		// remove trailing '\r' (Windows line endings)
		if (!buildingName.empty() && buildingName.back() == '\r')
		{
			buildingName.pop_back();
			buildingName = trim(buildingName);
		}

		// skip empty lines
		if (buildingName.empty())
		{
			cout << "File name: \"buildings.txt\", Line number: "<< lineNumber<< ", Reason: empty line." << endl;
			lineNumber++;
			continue;
		}

		// skip comments
		if (buildingName[0] == '#')
		{
			cout << "File name: \"buildings.txt\", Line number: "<< lineNumber<< ", Reason: comment line." << endl;
			lineNumber++;
			continue;
		}

		// skip invalid format
		if (!isBuildingNameValid(buildingName))
		{
			cout << "File name: \"buildings.txt\", Line number: "<< lineNumber<< ", Reason: malformed data." << endl;
			lineNumber++;
			continue;
		}

		if (isBuildingNameUnique(buildingName))
		{
			buildings.push_back(buildingName);
			lineNumber++;
			accepted++;
		}
		else
		{
			cout << "File name: \"buildings.txt\", Line number: "<< lineNumber<< ", Reason: duplicate." << endl;
			lineNumber++;
			rejected++;
			continue;	
		}
	}
	
	if (buildings.size() == 0)
	{
		cout << "File is empty." << endl;
		cout << "buildings.txt: 0 accepted, 0 rejected" << endl;
	}
	else
	{
		cout << "buildings.txt: " << accepted << " accepted, " << rejected << " rejected" << endl;
	}

	cout << endl;

	// close resources
	fin.close();
}


void Manager::loadFacilitiesFromFile(string fileName)
{
	ifstream fin;
	fin.open(fileName);
	
	// validates file-opening
	if (fin.fail())
	{
		cout << "File error: cannot open. Treated as empty." << endl;
		cout << "buildings.txt: 0 accepted, 0 rejected" << endl;
		return;
	}

	int accepted = 0;
	int rejected = 0;

	int lineNumber = 1;

	string line, FacilityID, FacilityName, BuildingName, Category;
	while (getline(fin, line))
	{
		
		// remove trailing '\r' (Windows line endings)
		if (!line.empty() && line.back() == '\r')
		{
			line.pop_back(); // removes the last symbol
			line = trim(line);
		}

		// skip empty lines
		if (line.empty())
		{
			cout << "File name: \"facilities.txt\", Line number: "<< lineNumber<< ", Reason: empty line." << endl;
			lineNumber++;
			continue;
		}

		// skip comments
		if (line[0] == '#')
		{
			cout << "File name: \"facilities.txt\", Line number: "<< lineNumber<< ", Reason: comment line." << endl;
			lineNumber++;
			continue;
		}

		stringstream ss(line);
		getline(ss, FacilityID, '|');
		getline(ss, FacilityName, '|');
		getline(ss, BuildingName, '|');
		getline(ss, Category, '|');

		FacilityID = trim(FacilityID);
		FacilityName = trim(FacilityName);
		BuildingName = trim(BuildingName);
		Category = trim(Category);

		// skip malformed lines
		if (FacilityID.empty() || FacilityName.empty() || BuildingName.empty() || Category.empty())
		{
			cout << "File name: \"facilities.txt\", Line number: "<< lineNumber<< ", Reason: malformed line." << endl;
			lineNumber++;
			continue;
		}

		if (!doesBuildingNameExist(BuildingName))
		{
			cout << "File name: \"facilities.txt\", Line number: "<< lineNumber<< ", Reason: building not found." << endl;
			lineNumber++;
			continue;
		}

		if (!isFacilityIDValid(FacilityID))
		{
			cout << "File name: \"facilities.txt\", Line number: "<< lineNumber<< ", Reason: invalid facility ID." << endl;
			lineNumber++;
			continue;
		}

		if (FacilityID[0] == 'f')
		{
			FacilityID[0] = 'F';
		}

		if (map.isDuplicate(FacilityID))
		{
			cout << "File name: \"facilities.txt\", Line number: "<< lineNumber<< ", Reason: duplicate is rejected." << endl;
			lineNumber++;
			rejected++;
			continue;
		}

		Facility facility(FacilityID, FacilityName, BuildingName, Category);

		map.countCollisions(FacilityID);
		map.insertItem(facility);
		accepted++;
		lineNumber++;
	}
	
	if (map.getSize() == 0)
	{
		cout << "File is empty." << endl;
		cout << "facilities.txt: 0 accepted, 0 rejected" << endl;
		cout << endl;
		return;
	}

	cout << "facilities.txt: " << accepted << ", " << rejected << " rejected (hash capacity " << map.getCapacity() << ", entries " << map.getSize() << ", collisions " << map.getNumberOfCollisions() << ")" << endl;
	cout << endl;

	// close resources
	fin.close();
}


// hash table use
void Manager::viewFacilities()
{
	map.viewAllFacilities();
}


void Manager::searchFacility()
{
	string facilityID;
	cout << "Facility ID: ";
	getline(cin, facilityID);

	facilityID = trim(facilityID);

	if (!isFacilityIDValid(facilityID))
	{
		cout << "Invalid format: an ID is F followed by exactly three digits" << endl;
		cout << endl;
		return;
	}

	if (facilityID[0] == 'f')
	{
		facilityID[0] = 'F';
		cout << "Same result as " << facilityID << " (input normalized to uppercase)" << endl;
	}

	map.searchItem(facilityID);
}