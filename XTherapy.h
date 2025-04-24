#pragma once
#include <iostream>
using namespace std;
#include "Treatment.h"

class XTherapy : public Treatment
{
public:
	XTherapy(Patient* patient = nullptr, int duration = 0, int assignmentTime = 0) : Treatment(patient, duration, GYM, assignmentTime) {}


	bool canAssign(Scheduler* s)
	{
		// TODO fix
		if (s->getXRooms().getCount() != 0)
			return true;
		else
			return false;
	}

	void moveToWait(Scheduler* s)
	{
		s->addToWaitX(patient);
	}
};