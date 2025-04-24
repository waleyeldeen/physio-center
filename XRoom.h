#pragma once
using namespace std;
#include <iostream>
#include "Resource.h"

class XRoom : public Resource
{
private:
	const int cap;
	int numOfPts;
public:
	XRoom(int id = 0, ResourceType type = NONE, int cap = 0) : Resource(id, type), cap(cap) 
	{
		numOfPts = 0;
	}


	int getCapacity() const
	{
		return cap;
	}

	int getNumOfPts() const
	{
		return numOfPts;
	}

	void incrementNumOfPts()
	{
		numOfPts++;
	}

	friend ostream& operator<<(ostream& os, const XRoom* x) {

		string z;
		if (x->avail == true)
			z = "Available";
		else
			z = "UnAvailable";

		os << x->id << '/' << z << '/' << x->cap;
		return os;
	}

	void makeAbstract() {}
};