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

class UI;

class Scheduler
{
private:
	int ts;

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

	//void addToIdle(Patient* p);

public:
	Scheduler();

	LinkedQueue<Patient*>& getIdle();
	EarlyPList& getEarly();
	PriQueue<Patient*>& getLate();

	UEWaitlist& getWaitU();
	UEWaitlist& getWaitE();
	XWaitlist& getWaitX();

	PriQueue<Patient*>& getServing();

	LinkedQueue<UDevice*>& getUDevices();
	LinkedQueue<EDevice*>& getEDevices();
	LinkedQueue<XRoom*>& getXRooms();

	ArrayStack<Patient*>& getFinish();

	//void addToEarly();
	//void addToLate();

	void addToWaitU(Patient* p);
	void addToWaitE(Patient* p);
	void addToWaitX(Patient* p);

	/*Opens the input file and assigs the data to different data members*/
	void loadInputFile(string fileName);

	//void runSimulation(UI* ui);
	void sim(UI* ui);

	// PHASE 2 FUNCS
	/*Called at each timestep to check the idle list for patients who
	arrived and move them to early or late accordingle. If VT==PT then movetowait func is called*/
	void moveArrivedPatients();
};

