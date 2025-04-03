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

};
