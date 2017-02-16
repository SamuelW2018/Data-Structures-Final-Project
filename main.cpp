#include <ctime>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include "Nurse.h"
#include "Doctor.h"
#include "EmergencyRoom.h"


int main()
{
	srand(time(NULL));

	EmergencyRoom e;
	e.LoadNamesFromFile();
	e.InitializeVariables();

	// main simulation loop
	for (int clock = 0; clock < e.GetNumOfMinutes(); ++clock)
	{
		int randInt = rand() % 100;
		if (randInt < e.GetPatientArrivalRate() * 100) // if a new patient arrives
			e.AddPatient(clock);
		e.TreatPatient(clock); // have available Physicians attempt to take a patient
		e.RemovePatient(clock); // remove patients who have finished treatment

	}

	e.DisplayMenu();
}

