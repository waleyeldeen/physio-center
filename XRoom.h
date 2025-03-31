#pragma once
using namespace std;
#include <iostream>
#include "Resource.h"

class XRoom : public Resource
{
public:
	XRoom(int id = 0, ResourceType type = NONE) : Resource(id, type) {}

};