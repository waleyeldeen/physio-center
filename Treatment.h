#pragma once
using namespace std;
#include <iostream>
#include "Resource.h"
#include "DEFS.h"

/*Forward Declaration*/
class Patient;
class Scheduler;

class Treatment
{
private:
	int duration, assignmentTime;
	const TreatmentType type;
	Resource* assignedRes;
protected:
	Patient* patient;
public:
	Treatment(Patient* patient = nullptr, int duration = 0, TreatmentType type = ULTRA, int assignmentTime = 0) : patient(patient), duration(duration), assignmentTime(assignmentTime), type(type), assignedRes(nullptr) {}

	// getters
	int getDuration()
	{
		return duration;
	}

	TreatmentType getType()
	{
		return type;
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
	virtual void moveToWait(Scheduler* s) = 0;
};