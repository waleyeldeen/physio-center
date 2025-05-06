#include "Scheduler.h"
#include "fstream"

#include "ETherapy.h"
#include "UTherapy.h"
#include "XTherapy.h"

Scheduler::Scheduler() { ts = 0; }

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

int Scheduler::getNumPatients() const { return numPatients; }
int Scheduler::getNumEarlyPatients() const { return numEarlyPatients; }
int Scheduler::getNumLatePatients() const { return numLatePatients; }

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

		Patient* newP = new Patient(this, i + 1, pt, vt, numTreatments, type);
		idle.enqueue(newP);

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

void Scheduler::addToWaitU(Patient* p)
{
	PatientStatus status = p->getStatus();
	if (status == ERLY)
		waitU.enqueue(p);
	else if (status == LATE || status == SERV)
		waitU.insertSorted(p);
}

void Scheduler::addToWaitE(Patient* p)
{
	PatientStatus status = p->getStatus();
	if (status == ERLY)
		waitE.enqueue(p);
	else if (status == LATE || status == SERV)
		waitE.insertSorted(p);
}

void Scheduler::addToWaitX(Patient* p)
{
	PatientStatus status = p->getStatus();
	if (status == ERLY)
		waitX.enqueue(p);
	else if (status == LATE || status == SERV)
		waitX.insertSorted(p);
}

// TODO: fix
void Scheduler::addToServe(Patient* p) {
	int ft = p->peekReqTreatment()->getDuration() + p->peekReqTreatment()->getAssignmentTime();
	serving.enqueue(p, -ft);
	p->updateWt(ts);
}

void Scheduler::sim(UI* ui)
{
	while (true)
	{
		ts++;
		moveUWaitPatientsToServe();
		moveEWaitPatientsToServe();
		moveXWaitPatientsToServe();

		rescAndCancelCaller();

		moveArrivedPatients();

		moveEarlyPatientsToWait();

		moveLatePatientsToWait();

		ui->printAllInformation(*this, ts);

		cin.get();
	}
}

bool Scheduler::rescAndCancelCaller()
{
	Patient* p = nullptr;
	int rescRand = getRandInRange(0, 100);
	int cancelRand = getRandInRange(0, 100);

	// call resc function
	// TODO: merge 2 if conditions in one condition
	if (rescRand < pResc)
	{
		if (early.reschedule(p)) // successfully rescheduled a patient
		{
			p->resced();
			cout << "Patient resced" << p->getId();
		}
	}

	if (cancelRand < pCancel)
	{
		if (waitX.pickRandCancelPatient(p))
		{
			p->canceled();
			cout << "Patient canceled" << p->getId();
		}
	}

	if (rescRand < pResc || cancelRand < pCancel)
		return true;
	return true;
}

void Scheduler::moveArrivedPatients()
{
    // case of 2 patients with same VT is handled using while loop
    while (true)
    {
        Patient* p = nullptr;
        idle.peek(p);
		if (p && ts == p->getVt())
		{
			// found an arrived patient
			idle.dequeue(p);
			int pt = p->getPt();
			int vt = p->getVt();

			// check if patient is early, late or VT==PT
			if (vt < pt)
			{
				// patient is early
				early.enqueue(p, -pt);
				p->setStatus(ERLY);

				numEarlyPatients++;
			}
			else if (vt > pt)
			{
				int penalty = (vt - pt) / 2;
				// patient is late
				late.enqueue(p, -pt);
				// apply penalty of half the difference
				p->setPenalty(penalty);
				// set new PT (old pt + penalty)
				p->setPt(pt + penalty);
				p->setStatus(LATE);

				numLatePatients++;
			}
			else if (vt == pt)
			{
				p->setStatus(ERLY);
				p->moveNextTreatmentToWait(ts);
				moveUWaitPatientsToServe();
				moveEWaitPatientsToServe();
				moveXWaitPatientsToServe();
			}
		}
		else
			break;
    }
}

void Scheduler::moveEarlyPatientsToWait()
{
	while (true)
	{
		Patient* p = nullptr;
		int pri = 0;
		early.peek(p, pri); // pri is negetive the appointment time
		if (p && ts == -pri)
		{
			// dequeue appoitned patient from early
			early.dequeue(p, pri);

			// there is a scheduler pointer in patient so there is no need to pass
			//   ts as an arguments but I made the decision to do so to make it more
			//   more intentional
			p->moveNextTreatmentToWait(ts);
		}
		else
			break;
	}
}

void Scheduler::moveLatePatientsToWait()
{
	while (true)
	{
		Patient* p = nullptr;
		int pri = 0;
		late.peek(p, pri); // pri is negetive the appointment time
		if (p && ts == p->getVt()+p->getPenalty())
		{
			// dequeue appoitned patient from early
			late.dequeue(p, pri);

			p->moveNextTreatmentToWait(ts);
		}
		else
			break;
	}
}

void Scheduler::getMinLatencyArray(TreatmentType arr[3])
{
	// Calculate latencies for each treatment type
	int latencies[3];
	latencies[0] = waitU.calcTreatmentLat(); // ULTRA
	latencies[1] = waitE.calcTreatmentLat(); // ELECTRO
	latencies[2] = waitX.calcTreatmentLat(); // GYM

	// Map indices to treatment types
	arr[0] = ULTRA;
	arr[1] = ELECTRO;
	arr[2] = GYM;

    // Sort the treatment types in ascending order of latency using a basic bubble sort
    bool swapped = true;
       while (swapped)
       {
           swapped = false;
           for (int i = 0; i < 2; ++i)
           {
               if (latencies[i] > latencies[i + 1])
               {
                   // Swap latencies  
                   int tempLatency = latencies[i];
                   latencies[i] = latencies[i + 1];
                   latencies[i + 1] = tempLatency;

                   // Swap corresponding treatment types  
                   TreatmentType tempType = arr[i];
                   arr[i] = arr[i + 1];
                   arr[i + 1] = tempType;

                   swapped = true;
               }
           }
       }
}

void Scheduler::moveUWaitPatientsToServe()
{
	Patient* p;
	Treatment* uTherapy;
	UDevice* uDevice;

	while (waitU.peek(p) && UTherapy::canAssign(this))
	{
		uTherapy = p->peekReqTreatment();
		waitU.dequeue(p);
		uDevices.dequeue(uDevice);

		uTherapy->setAssignmentTime(ts);
		uTherapy->setAssignedRes(uDevice);

		this->addToServe(p);
	}
}

void Scheduler::moveEWaitPatientsToServe()
{
	Patient* p;
	Treatment* eTherapy;
	EDevice* eDevice;

	while (ETherapy::canAssign(this) && waitE.peek(p))
	{
		eTherapy = p->peekReqTreatment();
		waitE.dequeue(p);
		eDevices.dequeue(eDevice);

		eTherapy->setAssignmentTime(ts);
		eTherapy->setAssignedRes(eDevice);

		this->addToServe(p);
	}
}

void Scheduler::moveXWaitPatientsToServe()
{
	Patient* p;
	Treatment* xTherapy;
	XRoom* xRoom;

	while (XTherapy::canAssign(this) && waitX.peek(p))
	{
		xTherapy = p->peekReqTreatment();
		waitX.dequeue(p);
		xRooms.peek(xRoom);

		xRoom->incrementNumOfPts();

		if (xRoom->getNumOfPts() == xRoom->getCapacity())
			xRooms.dequeue(xRoom);

		xTherapy->setAssignmentTime(ts);
		xTherapy->setAssignedRes(xRoom);

		this->addToServe(p);
	}
}