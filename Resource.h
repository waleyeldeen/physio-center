#pragma once
using namespace std;
#include <iostream>
#include "DEFS.h"


class Resource
{
protected:
	int id;
	ResourceType type;
	bool isAvailable;
public:
	Resource(int id = 0, ResourceType type = NONE) : id(id), type(type), isAvailable(true) {}

	// getters
	int getId() { return id; }
	ResourceType getType() { return type; }
	bool isAvailable() { return isAvailable; }

	// setters
	void setId(int newId) { id = newId; }

	void available() { isAvailable = true; }
	void unavailable() { isAvailable = false; }

	virtual void makeAbstract() = 0;
};

