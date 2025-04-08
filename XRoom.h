#pragma once
using namespace std;
#include <iostream>
#include "Resource.h"

class XRoom : public Resource
{
private:

	int cap;
public:
	XRoom(int id = 0, ResourceType type = NONE, int cap = 0) : Resource(id, type), cap(cap) {}

	friend ostream& operator<<(ostream& os, const XRoom* x) {

		string z;
		if (x->avail == true)
			z = "Available";
		else
			z = "UnAvailable";

		os << x->id << '/' << x->type << '/' << z << '/' << x->cap;
		return os;
	}

	void makeAbstract() {}
};