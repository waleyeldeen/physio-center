#pragma once
using namespace std;
#include <iostream>
#include "Resource.h"

class Treatment
{
private:
	int duration, time;
	Resource* assignedRes;
public:
	Treatment(int duration = 0, int time = 0) : duration(duration), time(time), assignedRes(nullptr) {}

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