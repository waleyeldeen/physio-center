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

	// getters
	int getDuration()
	{
		return duration;
	}


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

	virtual void canAssign() = 0;
	virtual void moveToWait() = 0;
};