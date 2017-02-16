#ifndef PHYSICIAN_H
#define PHYSICIAN_H



#include <iostream>
#include <vector>
#include "Patient.h"

class Physician
{
protected:
	Patient* CurrentPatient;
	std::vector<Patient*> TreatedPatients; // Number of patients that the physician has treated.
	int TreatmentTime; // clock time when current patient is done with treatment
	int InitialTime; // Clock time when the current patient is at the start of treatment.

	virtual void SetTreatmentTime(int clock) = 0; // Treatment time is defined in nurse and doctor.

public:
	Physician(Patient* CurrentPatient) : CurrentPatient(CurrentPatient) { } 

	Patient* GetCurrentPatient() { return CurrentPatient; } // Gets the current patient.
	int GetTreatmentTime() { return TreatmentTime; } // Returns treatment time (how long it takes for them to treat a patient).
	int GetInitialTime() { return InitialTime; } // Returns initial time (When they start treating a patient).
	int GetNumOfPatients() { return TreatedPatients.size(); } // Returns the number of patients that the physician has treated. 

	void SetPatient(Patient* P) { CurrentPatient = P; }
	void RemovePatient() { TreatedPatients.push_back(CurrentPatient); CurrentPatient = NULL; }
};

#endif // !PHYSICIAN_H