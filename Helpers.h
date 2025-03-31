#pragma once
#include <iostream>
using namespace std;

int getRandInRange(int low, int high)
{
	return low + rand() % (high - low + 1);
}
