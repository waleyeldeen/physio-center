#pragma once
using namespace std;
#include <iostream>
#include "DEFS.h"


class Resource
{
protected:
	int id;
	ResourceType type;
public:
	Resource(int id = 0, ResourceType type = NONE) : id(id), type(type) {}

	// getters
	int getId() { return id; }
	ResourceType getType() { return type; }

	// setters
	void setId(int newId) { id = newId; }

	friend ostream& operator<<(ostream& os, const Resource* r) {
		os << "[ID: " << r->id << ", " << "]";
		return os;
	}

	virtual void makeAbstract() = 0;
};

