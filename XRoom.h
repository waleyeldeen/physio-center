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
		if (numOfPts > cap)
			cerr << "ERROR in room Capacity";
	}

	void decrementNumOfPts()
	{
		numOfPts--;
		if (numOfPts < 0)
			cerr << "ERROR in room Capacity";
	}

	bool isFull()
	{
		if (cap == numOfPts) return true;
		return false;
	}

	friend ostream& operator<<(ostream& os, const XRoom* x) {
		os << "[ID: " << x->id << ", " << ", Cap: " << x->cap << ", Pts: " << x->getNumOfPts() << "]";
		return os;
	}

	void makeAbstract() {}
};