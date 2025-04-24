#pragma once
using namespace std;
#include <iostream>
#include "DEFS.h"


class Resource
{
protected:
	int id;
	ResourceType type;
	bool avail;
public:
	Resource(int id = 0, ResourceType type = NONE) : id(id), type(type), avail(true) {}

	// getters
	int getId() { return id; }
	ResourceType getType() { return type; }
	bool isAvailable() { return avail; }

	// setters
	void setId(int newId) { id = newId; }

	void available() { avail = true; }
	void unavailable() { avail = false; }

	friend ostream& operator<<(ostream& os, const Resource* r) {

		string z;
		if (r->avail == true)
			z = "Available";
		else
			z = "UnAvailable";

		os << "[ID: " << r->id << ", " << z << "]";
		return os;
	}

	virtual void makeAbstract() = 0;
};

