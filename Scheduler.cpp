#include "Scheduler.h"
#include "fstream"

#include "ETherapy.h"
#include "UTherapy.h"
#include "XTherapy.h"

Scheduler::Scheduler() { }

const LinkedQueue<Patient*>& Scheduler::getIdle() const { return idle; }
const EarlyPList& Scheduler::getEarly() const { return early; }
const PriQueue<Patient*>& Scheduler::getLate() const { return late; }

const UEWaitlist& Scheduler::getWaitU() const { return waitU; }
const UEWaitlist& Scheduler::getWaitE() const { return waitE; }
const XWaitlist& Scheduler::getWaitX() const { return waitX; }

const PriQueue<Patient*>& Scheduler::getServing() const { return serving; }

const LinkedQueue<UDevice*>& Scheduler::getUDevices() const { return uDevices; }
const LinkedQueue<EDevice*>& Scheduler::getEDevices() const { return eDevices; }
const LinkedQueue<XRoom*>& Scheduler::getXRooms() const { return xRooms; }

const ArrayStack<Patient*>& Scheduler::getFinish() const { return finish; }

void Scheduler::loadInputFile(string fileName)
{
	fstream file(fileName);

	// input number of devices
	int numEDevices, numUDevices, numXRooms;
	file >> numEDevices >> numUDevices >> numXRooms;
	
	// input capacities of gym rooms
	int* xCap = new int[numXRooms];
	for (int i = 0; i < numXRooms; i++)
		file >> xCap[i];

	// add device objects
	// Electro Devices
	for (int i = 0; i < numEDevices; i++)
	{
		// for the sake of the code I will make the id of each resource the i*10
		eDevices.enqueue(new EDevice((i+1) * 10, EDEVICE));
	}

	// Ultrasound Devices
	for (int i = 0; i < numUDevices; i++)
	{
		// for the sake of the code I will make the id of each resource the i*10
		uDevices.enqueue(new UDevice((i + 1) * 100, UDEVICE));
	}

	// Gym Rooms
	for (int i = 0; i < numXRooms; i++)
	{
		// for the sake of the code I will make the id of each resource the i*10
		xRooms.enqueue(new XRoom((i + 1) * 1000, XROOM));
	}

	// input pCancel and pResc
	file >> this->pCancel >> this->pResc;
	
	// input number of patients
	file >> numPatients;

	for (int i = 0; i < numPatients; i++)
	{
		char inType;
		file >> inType;
		bool type = (inType == 'N') ? true : false;

		int pt, vt, numTreatments;

		file >> pt >> vt >> numTreatments;

		Patient* newP = new Patient(i + 1, pt, vt, type);
		addToIdle(newP);

		for (int j = 0; j < numTreatments; j++)
		{
			char therapyType;
			file >> therapyType;

			int duration;
			file >> duration;

			switch (therapyType)
			{
				case 'E':
					newP->addTreatment(new ETherapy(duration)); break;
				case 'U':
					newP->addTreatment(new UTherapy(duration)); break;
				case 'X':
					newP->addTreatment(new XTherapy(duration)); break;
			}
		}
	}

	
	file.close();
}

void Scheduler::checkIdleForArrivedPatients()
{
	return;
}

void Scheduler::addToIdle(Patient* p) { idle.enqueue(p); }
void Scheduler::addToEarly() { Patient* p; idle.dequeue(p); early.enqueue(p, p->getPt()); }
void Scheduler::addToLate() { Patient* p; idle.dequeue(p); late.enqueue(p, p->getPt()); }

void Scheduler::addToWaitU(Patient* p) { waitU.enqueue(p); }
void Scheduler::addToWaitE(Patient* p) { waitE.enqueue(p); }
void Scheduler::addToWaitX(Patient* p) { waitX.enqueue(p); }