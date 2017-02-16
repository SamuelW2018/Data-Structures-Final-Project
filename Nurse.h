#ifndef NURSE_H
#define NURSE_H

#include <iostream>
#include <vector>
#include "Physician.h"

class Nurse : public Physician
{
public:
	Nurse() : Physician(NULL) { }

	// Nurse treatment time ranges from 1 to 10 and uses a random number generator.
	void SetTreatmentTime(int clock)
	{
		int Time = 0;
		Time = (rand() % 10) + 1;
		InitialTime = clock;
		TreatmentTime = clock + Time;
	}

};

#endif