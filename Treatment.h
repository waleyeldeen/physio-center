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
	int getDuration() const
	{
		return duration;
	}

	TreatmentType getType() const
	{
		return type;
	}


	void setAssignedRes(Resource* newResource)
	{
		assignedRes = newResource;
		assignedRes->unavailable();
	}

	void setAssignmentTime(int ts)
	{
		assignmentTime = ts;
	}

	// TODO: update it to make it do the check automatically
	void finishTreatment()
	{
		assignedRes->available();
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

	virtual bool canAssign(Scheduler* s) = 0;
	virtual void moveToWait(Scheduler* s) = 0;
};