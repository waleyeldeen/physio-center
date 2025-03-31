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
	void setId(int newId) { type = newId; }

	virtual void makeAbstract() = 0;
};

