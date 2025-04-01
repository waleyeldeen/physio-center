#pragma once
#include <iostream>
using namespace std;
#include "Treatment.h"

class XTherapy : public Treatment
{
public:
	XTherapy(int duration = 0, int assignmentTime = 0) : Treatment(duration, assignmentTime) {}


	void canAssign()
	{

	}

	void moveToWait()
	{

	}
};