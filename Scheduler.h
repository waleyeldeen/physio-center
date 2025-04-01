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
	LinkedQueue<Patient*> xRooms;

	ArrayStack<Patient*> finish;

	int pResc, pCancel, numPatients;

public:
	Scheduler();

	/*Called at each timestep to check the idle list for patients who
	arrived and move them to early or late accordingle*/
	void checkIdleForArrivedPatients();

	void addToIdle(Patient* p);
	void addToEarly(Patient* p);
	void addToLate(Patient* p);

	void addToWaitU(Patient* p);
	void addToWaitE(Patient* p);
	void addToWaitX(Patient* p);

	/*Opens the input file and assigs the data to different data members*/
	void loadInputFile(string fileName);

};

