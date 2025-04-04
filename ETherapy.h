#pragma once
#include <iostream>
using namespace std;
#include "Treatment.h"

class ETherapy : public Treatment
{
public:
	ETherapy(int duration = 0, int assignmentTime = 0) : Treatment(duration, assignmentTime) {}

	void canAssign()
	{

	}

	void moveToWait()
	{

	}
};