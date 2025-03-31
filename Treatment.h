#pragma once
using namespace std;
#include <iostream>
#include "Resource.h"

class Treatment
{
private:
	int duration, assignmentTime;
	Resource* assignedRes;
public:
	Treatment(int duration = 0, int assignmentTime = 0) : duration(duration), assignmentTime(assignmentTime), assignedRes(nullptr) {}

	void setAssignedRes(Resource* newResource)
	{
		assignedRes = newResource;
		assignedRes->unavailable();
	}

	void finishTreatment()
	{
		assignedRes->available();
	}

	virtual void dummyAbstract() = 0;
};