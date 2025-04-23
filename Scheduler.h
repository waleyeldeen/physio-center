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

	int ts = 0;

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

	/*Called at each timestep to check the idle list for patients who
	arrived and move them to early or late accordingle*/
	void checkIdleForArrivedPatients();

	void addToEarly();
	void addToLate();

	void addToWaitU(Patient* p);
	void addToWaitE(Patient* p);
	void addToWaitX(Patient* p);

	void addToServe(Patient* p);

	/*Opens the input file and assigs the data to different data members*/
	void loadInputFile(string fileName);

	void runSimulation(UI* ui);

	// Getter for TimeStep
	int GetTimeStep() const;

	// Assigning Patients with Treatments functions
	void Move_EWaitngPTs_to_Serve(); // Assign P with E
	void Move_UWaitingPTs_to_Serve(); // Assign P with U
	void Move_XWaitingPTs_to_Serve(); // Assign P with X

	void IncrementTimeStep();
};

