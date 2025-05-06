#pragma once
using namespace std;
#include <iostream>
#include "Resource.h"
#include "DEFS.h"

#include"XRoom.h"

/*Forward Declaration*/
class Patient;
class Scheduler;

class Treatment
{
private:
	int duration, assignmentTime;
	const TreatmentType type;
	Resource* assignedRes;

	XRoom* xroom = nullptr;
protected:
	Patient* patient;
public:
	Treatment(Patient* patient = nullptr, int duration = 0, TreatmentType type = ULTRA, int assignmentTime = 0) : patient(patient), duration(duration), assignmentTime(assignmentTime), type(type), assignedRes(nullptr) {}

	void setDuration(int x)
	{
		duration = x;
	}

	int getDuration() const
	{
		return duration;
	}

	TreatmentType getType() const
	{
		return type;
	}


	Patient* getPatient()
	{
		return patient;
	}

	
	void setAssignedRes(Resource* newResource)
	{
		assignedRes = newResource;
		if (newResource->getType() == GYM)
			return;
		assignedRes->unavailable();
	}

	Resource* getAssignedResource()
	{
		return assignedRes;
	}

	void setAssignmentTime(int ts)
	{
		assignmentTime = ts;
	}

	int getAssignmentTime() const
	{
		return assignmentTime;
	}

	// TODO: update it to make it do the check automatically
	void finishTreatment()
	{
		assignedRes->available();
	}

	void Set_X_Room(XRoom* x)
	{
		xroom = x;
	}

	XRoom* Get_X_Room()
	{
		return xroom;
	}

	// Output stream operator
	friend std::ostream& operator<<(std::ostream& os, const Treatment* p)
	{
		os << "<<";
		switch (p->type)
		{
		case ULTRA:
			os << "ULTRA"; break;
		case ELECTRO:
			os << "ELECTRO"; break;
		case  GYM:
			os << "GYM"; break;
		default:
			os << "NONE";
		}
		os << " " << p->duration << ">>";

		return os;
	}

    //static bool canAssign(Scheduler* s) = 0;
	virtual void moveToWait(Scheduler* s) = 0;
};