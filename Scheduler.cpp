#include "Scheduler.h"
#include "fstream"

#include "ETherapy.h"
#include "UTherapy.h"
#include "XTherapy.h"

Scheduler::Scheduler() { }

LinkedQueue<Patient*>& Scheduler::getIdle() { return idle; }
EarlyPList& Scheduler::getEarly() { return early; }
PriQueue<Patient*>& Scheduler::getLate() { return late; }

UEWaitlist& Scheduler::getWaitU() { return waitU; }
UEWaitlist& Scheduler::getWaitE() { return waitE; }
XWaitlist& Scheduler::getWaitX() { return waitX; }

PriQueue<Patient*>& Scheduler::getServing() { return serving; }

LinkedQueue<UDevice*>& Scheduler::getUDevices() { return uDevices; }
LinkedQueue<EDevice*>& Scheduler::getEDevices() { return eDevices; }
LinkedQueue<XRoom*>& Scheduler::getXRooms() { return xRooms; }

ArrayStack<Patient*>& Scheduler::getFinish() { return finish; }

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
		xRooms.enqueue(new XRoom((i + 1) * 1000, XROOM, xCap[i]));
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


        Treatment* t = nullptr;
        PatientStatus st = IDLE;
        

		Patient* newP = new Patient(this, i + 1, pt, vt, type, st, t);
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
					newP->addTreatment(new ETherapy(newP, duration)); break;
				case 'U':
					newP->addTreatment(new UTherapy(newP, duration)); break;
				case 'X':
					newP->addTreatment(new XTherapy(newP, duration)); break;
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
void Scheduler::addToEarly() { Patient* p; idle.dequeue(p); early.enqueue(p, -p->getPt()); }
void Scheduler::addToLate() { Patient* p; idle.dequeue(p); late.enqueue(p, -p->getPt()); }

void Scheduler::addToWaitU(Patient* p) { waitU.enqueue(p); }
void Scheduler::addToWaitE(Patient* p) { waitE.enqueue(p); }
void Scheduler::addToWaitX(Patient* p) { waitX.enqueue(p); }

void Scheduler::addToServe(Patient* p) { serving.enqueue(p, -p->peekReqTreatment()->getDuration() - ts); }

#include "UI.h"

int Scheduler::GetTimeStep() const { return ts; }

void Scheduler::Move_EWaitngPTs_to_Serve()
{
    ETherapy* ETh = new ETherapy();
    EDevice* E;
    Patient* Pt;
    while (ETh->canAssign(this) && this->getWaitE().getCount() != 0)
    {
        this->getWaitE().dequeue(Pt);
        this->getEDevices().dequeue(E);
        Pt->peekReqTreatment()->setAssignmentTime(ts);
        Pt->peekReqTreatment()->setAssignedRes(E);
        this->addToServe(Pt);
    }
}

void Scheduler::Move_UWaitingPTs_to_Serve()
{
    UTherapy* UTh = new UTherapy();
    UDevice* U;
    Patient* Pt;
    while (UTh->canAssign(this) && this->getWaitU().getCount() != 0)
    {
        this->getWaitU().dequeue(Pt);
        this->getUDevices().dequeue(U);
        Pt->peekReqTreatment()->setAssignmentTime(ts);
        Pt->peekReqTreatment()->setAssignedRes(U);
        this->addToServe(Pt);
    }

}

void Scheduler::Move_XWaitingPTs_to_Serve()
{
    XTherapy* XTh = new XTherapy();
    XRoom* X;
    Patient* Pt;
    while (XTh->canAssign(this) && this->getWaitX().getCount() != 0)
    {
        this->getWaitX().dequeue(Pt);
        this->getXRooms().peek(X);
        X->incrementNumOfPTsIn();
        if (X->getCounter() == X->getCapacity())
            this->getXRooms().dequeue(X);
        Pt->peekReqTreatment()->setAssignmentTime(ts);
        Pt->peekReqTreatment()->setAssignedRes(X);
        this->addToServe(Pt);
    }

}