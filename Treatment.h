#pragma once
using namespace std;
#include <iostream>

class Treatment
{
private:
	int duration, time;
public:
	Treatment(int duration = 0, int time = 0) : duration(duration), time(time) {}

	virtual void dummyAbstract() = 0;
};