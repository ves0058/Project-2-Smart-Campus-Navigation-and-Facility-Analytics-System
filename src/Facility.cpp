#include "Facility.h"


// default constructor
Facility::Facility()
{
	FacilityID = "";
	FacilityName = "";
	BuildingName = "";
	Category = "";
}


// parameterized constructor
Facility::Facility(string FacilityID, string FacilityName, string BuildingName, string Category)
{
	this->FacilityID = FacilityID;
	this->FacilityName = FacilityName;
	this->BuildingName = BuildingName;
	this->Category = Category;
}


// accessors (getters)
string Facility::getFacilityID() const
{
	return FacilityID;
}

string Facility::getFacilityName() const
{
	return FacilityName;
}

string Facility::getBuildingName() const
{
	return BuildingName;
}

string Facility::getCategory() const
{
	return Category;
}



// mutators (setters)
void Facility::setFacilityID(string FacilityID)
{
	this->FacilityID = FacilityID;
}

void Facility::setFacilityName(string FacilityName)
{
	this->FacilityName = FacilityName;
}

void Facility::setBuildingName(string BuildingName)
{
	this->BuildingName = BuildingName;
}

void Facility::setCategory(string Category)
{
	this->Category = Category;
}