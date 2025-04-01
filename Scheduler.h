#pragma once
#include <iostream>
using namespace std;

#include "LinkedQueue.h"
#include "EarlyPList.h"
#include "PriQueue.h"
#include "UEWaitlist.h"
#include "XWaitlist.h"
#include "ArrayStack.h"

class Scheduler
{
private:
	LinkedQueue<Patient*> idle;
	EarlyPList early;
	PriQueue<Patient*> late;

	UEWaitlist waitU;
	UEWaitlist waitE;
	XWaitlist waitX;

	PriQueue<Patient*> serving;

	LinkedQueue<Patient*> uDevices;
	LinkedQueue<Patient*> eDevices;
	LinkedQueue<Patient*> xDevices;

	ArrayStack<Patient*> finish;
};

