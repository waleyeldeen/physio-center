#pragma once
#include <iostream>
using namespace std;
#include "Treatment.h"

class XTherapy : public Treatment
{
public:
	XTherapy(Patient* patient = nullptr, int duration = 0, int assignmentTime = 0) : Treatment(patient, duration, GYM, assignmentTime) {}


	void canAssign()
	{

	}

	void moveToWait(Scheduler* s)
	{
		s->addToWaitE(patient);
	}
};