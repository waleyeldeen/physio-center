#include "Scheduler.h"
#include "fstream"

Scheduler::Scheduler() { }

void Scheduler::loadInputFile(string fileName)
{
	fstream file(fileName);
	string line;

	int numEDevices, numUDevices, numXRooms;
	file >> numEDevices >> numUDevices >> numXRooms;
	cout << "E: " << numEDevices << " U: " << numUDevices << " X: " << numXRooms;
}

void Scheduler::checkIdleForArrivedPatients()
{
	return;
}

void Scheduler::addToIdle(Patient* p) { idle.enqueue(p); }
void Scheduler::addToEarly(Patient* p) { early.enqueue(p, p->getPt()); }
void Scheduler::addToLate(Patient* p) { late.enqueue(p, p->getPt()); }

void Scheduler::addToWaitU(Patient* p) { waitU.enqueue(p); }
void Scheduler::addToWaitE(Patient* p) { waitE.enqueue(p); }
void Scheduler::addToWaitX(Patient* p) { waitX.enqueue(p); }