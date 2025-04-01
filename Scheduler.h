#pragma once
#include <iostream>
using namespace std;

#include "LinkedQueue.h"
#include "EarlyPList.h"
#include "PriQueue.h"
#include "UEWaitlist.h"
#include "XWaitlist.h"
#include "ArrayStack.h"

#include "EDevice.h"
#include "UDevice.h"
#include "XRoom.h"

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

	LinkedQueue<UDevice*> uDevices;
	LinkedQueue<EDevice*> eDevices;
	LinkedQueue<XRoom*> xRooms;

	ArrayStack<Patient*> finish;

	int pResc, pCancel, numPatients;

	void addToIdle(Patient* p);

public:
	Scheduler();

	/*Called at each timestep to check the idle list for patients who
	arrived and move them to early or late accordingle*/
	void checkIdleForArrivedPatients();

	void addToEarly(Patient* p);
	void addToLate(Patient* p);

	void addToWaitU(Patient* p);
	void addToWaitE(Patient* p);
	void addToWaitX(Patient* p);

	/*Opens the input file and assigs the data to different data members*/
	void loadInputFile(string fileName);

};

