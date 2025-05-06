#pragma once
#include <iostream>
using namespace std;

// TODO: should I use forward decs
#include "LinkedQueue.h"
#include "EarlyPList.h"
#include "PriQueue.h"
#include "UEWaitlist.h"
#include "XWaitlist.h"
#include "ArrayStack.h"

#include "EDevice.h"
#include "UDevice.h"
#include "XRoom.h"
#include "UI.h"
#include "fstream"
#include "iostream"

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

	int pResc, pCancel, numPatients, numEarlyPatients, numLatePatients;

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

	int getNumPatients() const;
	int getNumEarlyPatients() const;
	int getNumLatePatients() const;

	//void addToEarly();
	//void addToLate();

	/*
		functions called by Treatment::moveToWait()
		They depend on the patient status (ERLY, LATE, SERV)
		TODO: should they dequeue from the prevous one
	*/
	void addToWaitU(Patient* p);
	void addToWaitE(Patient* p);
	void addToWaitX(Patient* p);

	void addToServe(Patient* p);

	/*Opens the input file and assigs the data to different data members*/
	void loadInputFile(string fileName);

	//void runSimulation(UI* ui);
	void sim(UI* ui);

	/*
		call EarlyPList.cancel() and XWaitlist.cancel() according to pCancel and pResc
	*/
	bool rescAndCancelCaller();

	// PHASE 2 FUNCS
	/*Called at each timestep to check the idle list for patients who
	arrived and move them to early or late accordingle. If VT==PT then movetowait func is called*/
	void moveArrivedPatients();

	/*
		move patients in early to appropiate waiting
	*/
	void moveEarlyPatientsToWait();

	/*
		move patients in late to appropiate waiting
	*/
	void moveLatePatientsToWait();

	/*
		return an array sorted asc by latency of each waitlist
	*/
	void getMinLatencyArray(TreatmentType arr[3]);

	void moveUWaitPatientsToServe();
	void moveEWaitPatientsToServe();
	void moveXWaitPatientsToServe();

	void moveFromServeToWaitOrFinish();

	void outputFile();
};

