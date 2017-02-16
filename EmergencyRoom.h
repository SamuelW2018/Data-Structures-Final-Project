#ifndef EMERGENCY_H
#define EMERGENCY_H


#include <iostream>
#include <queue>
#include <list>
#include <map>
#include <vector>
#include <list>
#include <string>
#include "Patient.h"
#include "Physician.h"
#include "Record.h"

// copied from http://stackoverflow.com/questions/986021/how-to-implement-sorting-method-for-a-c-priority-queue-with-pointers
struct ptr_less {
	template<class T>
	bool operator()(const T& left, const T& right) const {
		return ((*left) <(*right));
	}
};

class EmergencyRoom
{
private:
	std::list<Physician*> Available_Physicians;
	std::list<Physician*> Unavailable_Physicians;
	std::priority_queue<Patient*, std::vector<Patient*>, ptr_less> High_Priority_Patients; // patients with priority 11-20
	std::priority_queue<Patient*, std::vector<Patient*>, ptr_less> Low_Priority_Patients; // patients with priority 1-10
	std::vector<std::string> TownspeopleNames;
	std::vector<bool> TownspeopleStatus; // true if currently in EmergencyRoom, false if not
	std::map<std::string, Record> HospitalRecord;

	double PatientArrivalRate; // average number of patients per minute
	int totalPatients; // total number of patients treated
	int timeSpent; // total treatment time for all patients
	int SickPeople; // Number of people currently either in the emergency room or in the queues.
	int NumOfMinutes; // number of minutes simulation will run

public:
	EmergencyRoom() : totalPatients(0), SickPeople(0), timeSpent(0), PatientArrivalRate(0), NumOfMinutes(0) { }

	void DisplayMenu();
	void LoadNamesFromFile();
	void InitializeVariables();

	void AddPatient(int clock);
	void RemovePatient(int clock);
	void TreatPatient(int clock);
	
	int GetDiseaseNumber();
	double GetPatientArrivalRate() { return PatientArrivalRate; }
	int GetNumOfMinutes() { return NumOfMinutes; }
	int GetTotalPatients() { return totalPatients; }
};

#endif