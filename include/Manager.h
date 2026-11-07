#ifndef MANAGER_H
#define MANAGER_H


#include "Facility.h"
#include "HashTable.h"
#include <sstream>
#include <cctype> // tolower, toupper
#include <iostream>
#include <fstream>


class Manager
{
	private:
		vector<string> buildings; // building names
		HashTable map; // hash table that stores facilities
	public:
		// "main.cpp" methods
		bool isChoiceCorrect(string input); // choice vailadtion
		void printHeader();
		void printMenu();

		// file-loading
		void loadBuildingsFromFile(string fileName);
		void loadFacilitiesFromFile(string fileName);
		string trim(string line);

		// validation and error-handling
		bool isBuildingNameValid(string BuildingName);
		bool isFacilityIDValid(string &FacilityID); // will change "f"->"F"
		bool isBuildingNameUnique(string buildingName); // pass by reference - does not change the values
		bool equalsIgnoreCase(string s1, string s2);
		bool doesBuildingNameExist(string BuildingName);

		// hash table use
		void viewFacilities();
		void searchFacility();
};



#endif