#pragma once
#include <iostream>
using namespace std;

inline int getRandInRange(int low, int high)
{
	return low + rand() % (high - low + 1);
}
