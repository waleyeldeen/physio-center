#pragma once
using namespace std;
#include <iostream>
#include "Resource.h"

class XRoom : public Resource
{
private:

	int cap;
	int counter = 0;
public:
	XRoom(int id = 0, ResourceType type = NONE, int cap = 0) : Resource(id, type), cap(cap) {}

	friend ostream& operator<<(ostream& os, const XRoom* x) {

		string z;
		if (x->avail == true)
			z = "Available";
		else
			z = "UnAvailable";

		os << x->id << '/' << z << '/' << x->cap;
		return os;
	}

	void setCapacity(int c)
	{
		cap = c;
	}

	int getCapacity()
	{
		return cap;
	}

	void setCounter(int x)
	{
		counter = x;
	}

	int getCounter()
	{
		return counter;
	}

	void incrementNumOfPTsIn()
	{
		counter++;
	}

	void makeAbstract() {}
};