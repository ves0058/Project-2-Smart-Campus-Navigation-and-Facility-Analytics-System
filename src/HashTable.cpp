#include "HashTable.h"


bool HashTable::equalsIgnoreCase(string s1, string s2)
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


// functions/methods
int HashTable::getNumberOfCollisions()
{
	return collisions;
}


int HashTable::getCapacity()
{
	return facilities.size();
}


int HashTable::getSize()
{
	return DSA_size;
}


void HashTable::countCollisions(string FacilityID)
{
	int index = hash(FacilityID);

	if (facilities.at(index).size() != 0)
	{
		collisions++;
	}
}


void HashTable::viewAllFacilities() // uses vector of keys to print all data in the input order
{
	if (DSA_size == 0)
	{
		cout << "Hash table is empty. The elements were not added." << endl;
		return;
	}

	vector<Facility>::iterator iter;
	for (int i = 0; i < (int) keys.size(); i++)
	{
		for (iter = facilities.at(keys.at(i)).begin(); iter != facilities.at(keys.at(i)).end(); iter++)
		{
			if (iter->getFacilityID() == IDs.at(i))
			{
				cout << iter->getFacilityID() << " | " << iter->getFacilityName() << " | " << iter->getBuildingName() << " | " << iter->getCategory() << endl;
			}
		}
	}

	cout << endl;
}


bool HashTable::isDuplicate(string FacilityID)
{
	int index = hash(FacilityID);

	vector<Facility>::iterator iter;

	for (iter = facilities.at(index).begin(); iter != facilities.at(index).end(); iter++)
	{
		if (equalsIgnoreCase(iter->getFacilityID(), FacilityID))
		{
			return true;
		}
	}

	return false;
}

// searching
void HashTable::searchItem(string FacilityID)
{
	int index = hash(FacilityID);

	vector<Facility>::iterator iter;

	for (iter = facilities.at(index).begin(); iter != facilities.at(index).end(); iter++)
	{
		if (iter->getFacilityID() == FacilityID)
		{
			cout << "Found: " << iter->getFacilityName() << " | " << iter->getBuildingName() << " | " << iter->getCategory() << endl;
			cout << endl;
			return;
		}
	}

	cout << "Not found: no facility with ID " << FacilityID << endl;
	cout << endl;
}


// insertion: use "Separate Chaining" technique to resolve collision problems
void HashTable::insertItem(Facility facility)
{
	int index = hash(facility.getFacilityID()); // get bucket using string hashing
	
	// maintaining the insertion order of elements
	keys.push_back(index);
	IDs.push_back(facility.getFacilityID());

	// insert a facility to the Hash Table
	facilities.at(index).push_back(facility);

	DSA_size++; // increase the counter of inserted elements
}


// deletion: removes element by the facility ID
void HashTable::deleteItem(string FacilityID)
{
	int index = hash(FacilityID);

	// remove facility from Hash Table
	vector<Facility>::iterator iter;
	for (iter = facilities.at(index).begin(); iter != facilities.at(index).end(); iter++)
	{
		if (iter->getFacilityID() == FacilityID)
		{
			facilities.at(index).erase(iter);
			DSA_size--; // decrease the counter of inserted elements
			break;
		}
	}

	// remove index from array of indices
	vector<int>::iterator itr;
	for (itr = keys.begin(); itr != keys.end(); itr++)
	{
		if (*itr == hash(FacilityID))
		{
			keys.erase(itr);
			break;
		}
	}

	// remove ID from array of facility IDs
	vector<string>::iterator it;
	for (it = IDs.begin(); it != IDs.end(); it++)
	{
		if (*it == FacilityID)
		{
			IDs.erase(it);
			break;
		}
	}
}


// string hashing function: Multiplicative String Hash Function (DJB2)
int HashTable::hash(string FacilityID)
{
	int initialValue = 5381;
	int multiplier = 33;
	int capacity = (int) facilities.size(); 

	int Hash = initialValue;
	
	char symbol;
	for (int i = 0; i < (int) FacilityID.size(); i++)
	{
		symbol = FacilityID[i]; // symbol (ASCII representation will be used to compute Hash)
		Hash = Hash * multiplier + symbol;
	}

	int index = Hash % capacity; // modulo: 101 (size of array is a prime number)

	return index;
}