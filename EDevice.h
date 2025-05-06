#pragma once
using namespace std;
#include <iostream>
#include "Resource.h"

class EDevice : public Resource
{
public:
	EDevice(int id = 0, ResourceType type = NONE) : Resource(id, type) {}

	int makeAbstract()
	{
		Index = 2;
		return Index;
	}
};