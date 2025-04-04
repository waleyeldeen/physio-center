#pragma once
using namespace std;
#include <iostream>
#include "Scheduler.h"

class UI
{
private:
	bool isInteractive;
public:
	UI(bool isInteractive = true);

	/*
		Gets input file name
	*/
	string inInputFileName();

	/*
		Prints all the lists in the Scheduler object
	*/
	void printAllInformation(Scheduler& sObj, int ts);
};
