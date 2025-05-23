#pragma once
#include <iostream>
using namespace std;
#include "Treatment.h"

class UTherapy : public Treatment
{
public:
	UTherapy(Patient* patient = nullptr, int duration = 0, int assignmentTime = 0) : Treatment(patient, duration, ULTRA, assignmentTime) {}


	static bool canAssign(Scheduler* s)
	{
		if (s->getUDevices().getCount() != 0)
			return true;
		else
			return false;
	}

	void moveToWait(Scheduler* s)
	{
		s->addToWaitU(patient);
	}
};