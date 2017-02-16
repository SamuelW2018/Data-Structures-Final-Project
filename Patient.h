#ifndef PATIENT_H
#define PATIENT_H


#include <iostream>
#include <string>

class Patient
{
private:
	int priorityNumber; // Disease priority number.
	std::string Name; // The patient's name.
	int ID; // Their ID number used for vector related purposes.
	int EnterTime; // Clock time for when the patient enters: used to calculate patient wait time.

public:
	Patient(int priorityNumber, std::string Name, int ID, int enter) : priorityNumber(priorityNumber), 
		Name(Name), ID(ID), EnterTime(enter) { }

	// Getters and setters for all the private variables.
	std::string GetName() { return Name; }
	
	void SetName(std::string name)
	{
		Name = name;
	}

	int GetPriorityNumber() const { return priorityNumber; }

	int GetEnterTime() const { return EnterTime; }

	void SetPriorityNumber(int PNum)
	{
		priorityNumber = PNum;
	}

	int GetIDNum() { return ID; }

	void SetIDNum(int ID)
	{
		this->ID = ID;
	}

	// Operator comparison function used to make sure the priority queue does its thing.
	bool operator< (const Patient& rhs) const
	{
		return priorityNumber < rhs.GetPriorityNumber();
	}
};



#endif