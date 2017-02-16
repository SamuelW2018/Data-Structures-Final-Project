#ifndef RECORD_H
#define RECORD_H

#include <iostream>
#include <vector>

class Record
{
private:
	int NumberOfVisits; // number of times patient visited emergency room
	std::vector<int> PriorityNumber; // priority number of each visit

public:
	Record(int NumVisits, int priorityNumber) : NumberOfVisits(NumVisits), PriorityNumber(1, priorityNumber) { };
	Record() : NumberOfVisits(0), PriorityNumber(0, 0) {};
	int GetVisits() { return NumberOfVisits; }
	std::vector<int> GetPriorNumber() { return PriorityNumber; }

	// Enhance the record by increasing the number of diseases that the person had.
	void AddToRecord(int PNum)
	{
		NumberOfVisits++;
		PriorityNumber.push_back(PNum);
	}
};



#endif