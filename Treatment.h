#pragma once
using namespace std;
#include <iostream>
#include "Resource.h"

/*Forward Declaration*/
class Patient;
class Scheduler;

class Treatment
{	
protected:
	int duration, assignmentTime;
	Resource* assignedRes;
	Patient* patient;
public:
	Treatment(Patient* patient = nullptr, int duration = 0, int assignmentTime = 0, Resource* AssignedRes = nullptr) : patient(patient), duration(duration), assignmentTime(assignmentTime), assignedRes(AssignedRes) {}

	// getters
	int getDuration()
	{
		return duration;
	}
	int getAssignmentTime()
	{
		return assignmentTime;
	}
	Resource* getAssignedResource()
	{
		return assignedRes;
	}
	Patient* getPatient()
	{
		return patient;
	}

	// setters
	void setDuration(int x)
	{
		duration = x;
	}
	void setAssignmentTime(int y)
	{
		assignmentTime = y;
	}
	void setAssignedRes(Resource* newResource)
	{
		assignedRes = newResource;
		assignedRes->unavailable();
	}
	void setPatient(Patient* P)
	{
		patient = P;
	}

	
	void finishTreatment(int TimeStep)
	{

			patient = nullptr;
			duration = 0;
			assignmentTime = 0;
			assignedRes->available();

	}

	virtual bool canAssign(Scheduler* s) = 0;
	virtual void moveToWait(Scheduler* s) = 0;
};