#ifndef FACILITY_H
#define FACILITY_H


#include <string>
#include <iostream>
using namespace std;


class Facility
{
	private:
		// attrubutes
		string FacilityID;
		string FacilityName;
		string BuildingName;
		string Category;
	public:
		// default constructor
		Facility();

		// parameterized constructor
		Facility(string FacilityID, string FacilityName, string BuildingName, string Category);

		// getters/accessors
		string getFacilityID() const;
		string getFacilityName() const;
		string getBuildingName() const;
		string getCategory() const;

		// setters/mutators
		void setFacilityID(string FacilityID);
		void setFacilityName(string FacilityName);
		void setBuildingName(string BuildingName);
		void setCategory(string Category);
};


#endif