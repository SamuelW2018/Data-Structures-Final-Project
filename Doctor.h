#ifndef DOCTOR_H
#define DOCTOR_H

#include <iostream>
#include <vector>
#include "Physician.h"
#include <ctime>

class Doctor : public Physician
{
public:
	Doctor() : Physician(NULL) { }

	// Doctor treatment time ranges from 1 to 20 and uses a random number generator.
	void SetTreatmentTime(int clock)
	{
		int Time = 0;
		Time = (rand() % 20) + 1;
		InitialTime = clock;
		TreatmentTime = clock + Time;
	}
};

#endif