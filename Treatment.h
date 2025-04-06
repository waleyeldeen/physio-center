#pragma once
using namespace std;
#include <iostream>
#include "Resource.h"

/*Forward Declaration*/
class Patient;

class Treatment
{
private:
	int duration, assignmentTime;
	Resource* assignedRes;
protected:
	Patient* patient;
public:
	Treatment(Patient* patient = nullptr, int duration = 0, int assignmentTime = 0) : patient(patient), duration(duration), assignmentTime(assignmentTime), assignedRes(nullptr) {}

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