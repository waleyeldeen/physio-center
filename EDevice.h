#pragma once
using namespace std;
#include <iostream>
#include "Resource.h"

class EDevice : public Resource
{
public:
	EDevice(int id = 0, ResourceType type = NONE) : Resource(id, type) {}

	void makeAbstract() { }
};