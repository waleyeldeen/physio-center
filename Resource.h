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
	int getId() { return id; }
	ResourceType getType() { return type; }

	virtual void makeAbstract() = 0;
};

