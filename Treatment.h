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

	// TODO: update it to make it do the check automatically
	void finishTreatment()
	{
		assignedRes->available();
	}

	virtual void CanAssign() = 0;
	virtual void MoveToWait() = 0;
};