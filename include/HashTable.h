#ifndef HASH_TABLE_H
#define HASH_TABLE_H


#include "Facility.h"
#include <string>
#include <vector>
using namespace std;


class HashTable
{
	private:
		// dynamic-array: initial capacity is set in constructor parameter
		vector<vector<Facility>> facilities{101}; // use prime number as a capacity to distribute the elements evenly
		
		int DSA_size = 0; // number of facilities in hash table

		int collisions = 0; // number of collisions encountered during loading for accepted insertions

		vector<int> keys; // keys in input order: orderbucket indices
		vector<string> IDs; // IDs in input order: facility IDs
	public:
		// functions/methods
		bool equalsIgnoreCase(string s1, string s2);
		int getNumberOfCollisions();
		int getCapacity();
		int getSize();
		void viewAllFacilities(); // uses vector of keys to print all data in the input order
		bool isDuplicate(string FacilityID);
		void countCollisions(string FacilityID);

		// 3 main methods of a Hash Table
		void searchItem(string FacilityID);
		void insertItem(Facility facility);
		void deleteItem(string FacilityID);
		
		// hash-function: Multiplicative String Hash Function (DJB2)
		int hash(string FacilityID);
};


#endif