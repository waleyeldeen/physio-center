#pragma once
#include <iostream>
using namespace std;
#include "Treatment.h"

class UTherapy : public Treatment
{
public:
	UTherapy(int duration = 0, int assignmentTime = 0) : Treatment(duration, assignmentTime) {}


	void canAssign()
	{

	}

	void moveToWait()
	{

	}
};