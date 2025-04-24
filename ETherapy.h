#pragma once
#include <iostream>
using namespace std;
#include "Treatment.h"

class ETherapy : public Treatment
{
public:
	ETherapy(Patient* patient = nullptr, int duration = 0, int assignmentTime = 0) : Treatment(patient, duration, ELECTRO, assignmentTime) {}

	static bool canAssign(Scheduler* s)
	{
		if (s->getEDevices().getCount() != 0)
			return true;
		else
			return false;
	}

	void moveToWait(Scheduler* s)
	{
		s->addToWaitE(patient);
	}
};