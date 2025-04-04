#pragma once
using namespace std;
#include <iostream>
#include "Resource.h"

class UDevice : public Resource
{
public:
	UDevice(int id = 0, ResourceType type = NONE) : Resource(id, type) {}

	void makeAbstract() {}
};