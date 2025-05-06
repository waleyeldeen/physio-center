#pragma once
using namespace std;
#include <iostream>
#include "Resource.h"

class XRoom : public Resource
{
private:

	int cap;
	int counter = 0;
	int Tools_1 = 0;
	int Tools_2 = 0;
	int Tools_3 = 0;

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

	void decrementNumOfPTsIn()
	{
		counter--;
	}

	void increment_Tool_1()
	{
		Tools_1++;
	}

	void increment_Tool_2()
	{
		Tools_2++;
	}

	void increment_Tool_3()
	{
		Tools_3++;
	}

	int GetSumOfTools()
	{
		return Tools_1 + Tools_2 + Tools_3;
	}

	int GetNumOfTool_1()
	{
		return Tools_1;
	}

	int GetNumOfTool_2()
	{
		return Tools_2;
	}

	int GetNumOfTool_3()
	{
		return Tools_3;
	}

	void decrement_Tool_1()
	{
		Tools_1--;
	}
	void decrement_Tool_2()
	{
		Tools_2--;
	}
	void decrement_Tool_3()
	{
		Tools_3--;
	}

	int makeAbstract()
	{
		Index = 3;
		return Index;
	}

	void SetTool_1(int x)
	{
		Tools_1 = x;
	}

	void SetTool_2(int y)
	{
		Tools_2 = y;
	}

	void SetTool_3(int z)
	{
		Tools_3 = z;
	}
};