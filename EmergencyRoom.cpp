#include "EmergencyRoom.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include "Patient.h"
#include "Nurse.h"
#include "Doctor.h"


// Loads the townspeople names from residents_273ville.txt
void EmergencyRoom::LoadNamesFromFile()
{
	std::ifstream fin;
	fin.open("residents_273ville.txt");

	if (!fin) {
		std::cout << "Error in opening file" << std::endl;
		return;
	}

	std::string line;
	while (!fin.eof())
	{
		fin >> line;
		TownspeopleNames.push_back(line);
	}

	fin.close();

	TownspeopleStatus.assign(TownspeopleNames.size(), false);
}

// Initializes starting variables based on user input
void EmergencyRoom::InitializeVariables()
{
	int NumNurses, NumDoctors, SimTime;
	bool IsError = false;
	double ArrivalRate;
	
	// Try-Catch to make sure that people do not break the program with a non-int object.
	do {
		std::cin.exceptions(std::istream::failbit);
		IsError = false;
		try
		{
			std::cout << "How many nurses are there in the hospital? \n";
			std::cin >> NumNurses; 
			std::cout << "How many doctors are there in the hospital? \n";
			std::cin >> NumDoctors;
			std::cout << "How many hours will the simulation run? \n";
			std::cin >> SimTime;
			do	{
				std::cout << "How many patients will arrive each hour? \n";
				std::cin >> ArrivalRate;
			} while (ArrivalRate >= 60 || ArrivalRate <= 0);
		}
		catch (std::ios_base::failure &fail)
		{
			std::cout << "Invalid input. \n";
			std::cout << fail.what() << std::endl;
			IsError = true;
			std::cin.clear();
			std::cin.ignore(256, '\n');
		}
		
	} while (IsError != false);

	

	ArrivalRate /= 60.0;
	PatientArrivalRate = ArrivalRate;

	NumOfMinutes = SimTime * 60;

	for (int i = 0; i < NumNurses; i++)
	{
		Physician* TNurse = new Nurse;
		Available_Physicians.push_back(TNurse);
	}

	for (int j = 0; j < NumDoctors; j++)
	{
		Physician* TDoctor = new Doctor;
		Available_Physicians.push_back(TDoctor);
	}
}

// Adds a new Patient to one of the queues
void EmergencyRoom::AddPatient(int clock)
{
	if (SickPeople < TownspeopleNames.size())
	{
		int randIndex;
		do {
			randIndex = (rand() % TownspeopleNames.size());
		} while (TownspeopleStatus[randIndex] == true); // if patient is already in EmergencyRoom

		int diseaseNumber = GetDiseaseNumber();
		Patient* newPatient = new Patient(diseaseNumber, TownspeopleNames[randIndex], randIndex, clock);
		TownspeopleStatus[randIndex] = true;
		SickPeople++;

		if (newPatient->GetPriorityNumber() <= 10)
			Low_Priority_Patients.push(newPatient);
		else
			High_Priority_Patients.push(newPatient);
	}

}

// Generates a random disease number
// 70% chance of 1-10, 20% chance of 11-15, and 10% chance of 16-20
int EmergencyRoom::GetDiseaseNumber()
{
	int randNum = (rand() % 10);
	if (randNum < 7) {
		return ((rand() % 10) + 1);
	}
	else if (randNum >= 7 && randNum < 9) {
		return ((rand() % 5) + 11);
	}
	else {
		return ((rand() % 5) + 16);
	}
}

// Lets a Doctor or Nurse take a patient from one of the queues
void EmergencyRoom::TreatPatient(int clock)
{
	if (Available_Physicians.empty())
	{
		return;
	}
	else if (!Low_Priority_Patients.empty())
	{
		for (std::list<Physician*>::iterator iter = Available_Physicians.begin(); iter != Available_Physicians.end(); 
			/*only want to increment if erase isn't called */)
		{
			if (typeid(**iter) == typeid(Nurse))
			{
				Nurse* N = dynamic_cast<Nurse*>(*iter);
				N->SetPatient(Low_Priority_Patients.top());
				Low_Priority_Patients.pop();
				N->SetTreatmentTime(clock);
				Unavailable_Physicians.push_back(*iter);
				iter = Available_Physicians.erase(iter); // iter = position after erased physician
			}
			
			else
				++iter;
			if (Low_Priority_Patients.empty())
			{
				break;
			}
		}
		for (std::list<Physician*>::iterator iter = Available_Physicians.begin(); iter != Available_Physicians.end(); 
			/*only want to increment if erase isn't called */)
		{
			if (typeid(**iter) == typeid(Doctor))
			{
				Doctor* D = dynamic_cast<Doctor*>(*iter);
				if (!High_Priority_Patients.empty())
				{
					D->SetPatient(High_Priority_Patients.top());
					High_Priority_Patients.pop();
					D->SetTreatmentTime(clock);
					Unavailable_Physicians.push_back(*iter);
					iter = Available_Physicians.erase(iter); // iter = position after erased physician
				}
				else if (!Low_Priority_Patients.empty())
				{
					D->SetPatient(Low_Priority_Patients.top());
					Low_Priority_Patients.pop();
					D->SetTreatmentTime(clock);
					Unavailable_Physicians.push_back(*iter);
					iter = Available_Physicians.erase(iter); // iter = position after erased physician
				}
				else
				{
					++iter; // not sure if necessary
					return;
				}
			}
			else
				++iter; // ensures iter is incremented
		}

	}
}

// Removes a patient if they're done being treated
void EmergencyRoom::RemovePatient(int clock)
{
	for (std::list<Physician*>::iterator iter = Unavailable_Physicians.begin(); iter != Unavailable_Physicians.end();
		/*only want to increment if erase isn't called*/)
	{
		if (typeid(**iter) == typeid(Nurse))
		{
			Nurse* N = dynamic_cast<Nurse*>(*iter);
			if (N->GetTreatmentTime() == clock)
			{
				std::string patientName = N->GetCurrentPatient()->GetName();
				int patientID = N->GetCurrentPatient()->GetIDNum();
				// add patient info to HospitalRecord
				std::map<std::string, Record>::iterator T = HospitalRecord.find(patientName);
				// Checks if patient is already in the record or not. If they are, the record is updated. Otherwise, they are added to the hospital record map.
				if (T != HospitalRecord.end())
				{
					T->second.AddToRecord(N->GetCurrentPatient()->GetPriorityNumber());
				}
				else
				{
					HospitalRecord.insert(std::pair<std::string, Record>(patientName, Record(1, N->GetCurrentPatient()->GetPriorityNumber())));
				}
				int PatientEnterTime = N->GetCurrentPatient()->GetEnterTime();
				N->RemovePatient();
				TownspeopleStatus[patientID] = false;
				SickPeople--;
				Available_Physicians.push_back(*iter); // Physician is now Available
				iter = Unavailable_Physicians.erase(iter); // iter = position after erased physician
				++totalPatients;
				// add to total treatment time
				timeSpent += (N->GetTreatmentTime() - PatientEnterTime);
			}
			else
				++iter;
		}
		else if (typeid(**iter) == typeid(Doctor))
		{
			Doctor* D = dynamic_cast<Doctor*>(*iter);
			if (D->GetTreatmentTime() == clock)
			{
				std::string patientName = D->GetCurrentPatient()->GetName();
				int patientID = D->GetCurrentPatient()->GetIDNum();
				// add patient info to HospitalRecord
				std::map<std::string, Record>::iterator T = HospitalRecord.find(patientName);
				// Checks if patient is already in the record or not. If they are, the record is updated. Otherwise, they are added to the hospital record map.
				if (T != HospitalRecord.end())
				{
					T->second.AddToRecord(D->GetCurrentPatient()->GetPriorityNumber());
				}
				else
				{
					HospitalRecord.insert(std::pair<std::string, Record>(patientName, Record(1, D->GetCurrentPatient()->GetPriorityNumber())));
				}
				int PatientEnterTime = D->GetCurrentPatient()->GetEnterTime();
				D->RemovePatient();
				TownspeopleStatus[patientID] = false;
				SickPeople--;
				Available_Physicians.push_back(*iter); // Physician is now Available
				iter = Unavailable_Physicians.erase(iter); // iter = position after erased physician
				++totalPatients;
				// add to total treatment time
				timeSpent += (D->GetTreatmentTime() - PatientEnterTime);
			}
			else
				++iter;
		}
		else
			++iter;
	}
}

// Displays stats and lets user look up patient records
void EmergencyRoom::DisplayMenu()
{
	std::cout << "There were a total of " << totalPatients << " visits to the emergency room by " << HospitalRecord.size() << " unique patients. \n";
	std::cout << "The total visit time was " << timeSpent << " minutes. \n";
	std::cout << "The average visit time was " << (timeSpent / totalPatients) << " minutes. \n";
	std::cout << "The number of untreated patients was " << ( High_Priority_Patients.size() + Low_Priority_Patients.size() ) << ". \n";

	int NurseCount = 1;
	int DoctorCount = 1;
	// Allows us to see how much each individual nurse and doctor treats.
	for (std::list<Physician*>::iterator iter = Available_Physicians.begin(); iter != Available_Physicians.end();
		++iter)
	{
		if (typeid(**iter) == typeid(Nurse))
		{
			Nurse* N = dynamic_cast<Nurse*>(*iter);
			std::cout << N->GetNumOfPatients() << " patients treated by Nurse " << NurseCount << ". \n";
			++NurseCount;
		}
		else if (typeid(**iter) == typeid(Doctor))
		{
			Doctor* D = dynamic_cast<Doctor*>(*iter);
			std::cout << D->GetNumOfPatients() << " patients treated by Doctor " << DoctorCount << ". \n";
			++DoctorCount;
		}
	}
	// Continuation of previous for loop, this time checking the physicians that are currently treating patients.
	for (std::list<Physician*>::iterator iter = Unavailable_Physicians.begin(); iter != Unavailable_Physicians.end();
		++iter)
	{
		if (typeid(**iter) == typeid(Nurse))
		{
			Nurse* N = dynamic_cast<Nurse*>(*iter);
			std::cout << N->GetNumOfPatients() << " patients treated by Nurse " << NurseCount << ". \n";
			++NurseCount;
		}
		else if (typeid(**iter) == typeid(Doctor))
		{
			Doctor* D = dynamic_cast<Doctor*>(*iter);
			std::cout << D->GetNumOfPatients() << " patients treated by Doctor " << DoctorCount << ". \n";
			++DoctorCount;
		}
	}
	// Menu asking user to view a list of treated patients, look up individual patient records, or quit the program.
	std::string answer = "Y";
	while (answer == "Y" || answer == "Yes")
	{
		std::string name;
		int choice;
		std::cout << "Do you want to view a list of all treated patients (1), look up a record by name (2), or quit the program? (3)";
		try
		{
			std::cin >> choice;
		}
		catch (std::ios_base::failure &fail)
		{
			std::cout << "Invalid input. \n";
			std::cin.clear();
			std::cin.ignore(256, '\n');
			choice = 3;
		}
		switch (choice)
		{
			case 1:
			{
				for (std::map<std::string, Record>::iterator T = HospitalRecord.begin(); T != HospitalRecord.end(); ++T)
					std::cout << T->first << " was treated " << T->second.GetVisits() << " times.\n";
				break;
			}
			case 2:
			{
				std::cout << "Enter the name of the patient you want to look up: \n";
				std::cin >> name;
				std::map<std::string, Record>::iterator T = HospitalRecord.find(name);
				if (T != HospitalRecord.end())
				{
					std::cout << name << "Has visited the emergency room " << HospitalRecord[name].GetVisits() << " times. \n";
					for (int i = 0; i < HospitalRecord[name].GetVisits(); i++)
					{
						std::cout << " On the " << i + 1 << "th visit the patient had an illness priority of " << HospitalRecord[name].GetPriorNumber()[i] << ". \n";
					}
				}
				else
				{
					std::cout << "Cannot find " << name << " in the record. \n";
				}
				break;
			}
			default:
			{
				break;
			}
		}
		
		std::cout << "Would you like to make another choice on the menu? (Y or N) \n";
		std::cin >> answer;
	}

	std::cout << "Goodbye! \n";
}