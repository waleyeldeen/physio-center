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

void Scheduler::IncrementTimeStep()
{
    ts++;
}

void Scheduler::runSimulation(UI* ui)
{
    
    while (ts != -1)
    {
        cout << "################################################################" << endl;
        ts++;
        Patient* p = nullptr;
        if (getIdle().peek(p))
        {
            // check if a patient has arrived
            if (p->getVt() == ts)
            {
                // check if the patient is late or early
                if (p->getPt() >= p->getVt())
                    addToEarly();
                else
                    addToLate();
            }
        }

        /*
            Random Waiting Procedure
        */
        TherapyType therapy;
        int chooseTherapy = getRandInRange(0, 100);
        if (chooseTherapy < 33)
            therapy = ELECTRO;
        else if (chooseTherapy < 66)
            therapy = ULTRA;
        else
            therapy = GYM;

        int x = getRandInRange(0, 100);

        Patient* rp = nullptr;
        int pri;

        if (x < 10)
        {
            cout << "####  MOVING NEXT PATIENT FROM EARLY TO RANDOMWAITING  ####" << endl;
            // dequeue next patient from early and get pointer to it
            if (getEarly().dequeue(rp, pri))
            {
                switch (therapy)
                {
                case ELECTRO:
                    getWaitE().enqueue(rp); break;
                case ULTRA:
                    getWaitU().enqueue(rp); break;
                case GYM:
                    getWaitX().enqueue(rp); break;
                }
            }
        }
        else if (x < 20)
        {
            cout << "####  MOVING NEXT PATIENT FROM LATE TO RANDOMWAITING  ####" << endl;

            // dequeue next patient from late and get pointer to it
            if (getLate().dequeue(rp, pri))
            {
                int penalty = (rp->getVt() - rp->getPt()) / 2;
                int newPt = penalty + rp->getPt();
                rp->setPt(newPt);
                switch (therapy)
                {
                case ELECTRO:
                    getWaitE().insertSorted(rp); break;
                case ULTRA:
                    getWaitU().insertSorted(rp); break;
                case GYM:
                    getWaitX().insertSorted(rp); break;
                }
            }
        }
        else if (x < 40)
        {
            cout << "####  MOVING 2 NEXT PATIENTS FROM RANDOMWAITING TO SERVING  ####" << endl;

            // move 2 next patients from a RandomWaiting to serving list
            Patient* rp2 = nullptr;

            bool getPatient1, getPatient2;

            switch (therapy)
            {
            case ELECTRO:
                getPatient1 = getWaitE().dequeue(rp);
                getPatient2 = getWaitE().dequeue(rp2);
                break;
            case ULTRA:
                getPatient1 = getWaitU().dequeue(rp);
                getPatient2 = getWaitU().dequeue(rp2);
                break;
            case GYM:
                getPatient1 = getWaitU().dequeue(rp);
                getPatient2 = getWaitU().dequeue(rp2);
            }

            if (getPatient1)
            {
                // TODO: should we do a minus one to finish time or not
                // get duration of next treatment in reqTreatment list (trivial)
                int duration = rp->peekReqTreatment()->getDuration();
                int finishTime = ts + duration;
                getServing().enqueue(rp, -finishTime);
                if (getPatient2)
                {
                    duration = rp2->peekReqTreatment()->getDuration();
                    finishTime = ts + duration;
                    getServing().enqueue(rp2, -finishTime);
                }
            }

        }
        else if (x < 50)
        {
            cout << "####  MOVING NEXT PATIENT FROM SERVING TO RANDOMWAITING  ####" << endl;

            //TODO: should I use insertSorted or enqueue (knowing that the later breaks the PT sort in waitlist)
            if (getServing().dequeue(rp, pri))
            {
                switch (therapy)
                {
                case ELECTRO:
                    getWaitE().insertSorted(rp); break;
                case ULTRA:
                    getWaitU().insertSorted(rp); break;
                case GYM:
                    getWaitX().insertSorted(rp); break;
                }
            }
        }
        else if (x < 60)
        {
            cout << "####  MOVING NEXT PATIENT FROM SERVING TO FINISH  ####" << endl;

            if (getServing().dequeue(rp, pri))
            {
                getFinish().push(rp);
            }
        }
        else if (x < 70)
        {
            cout << "####  MOVING RANDOM PATIENT FROM XWAITING TO FINISH  ####" << endl;

            rp = getWaitX().pickRandCancelPatient();
            if (rp != nullptr)
            {
                getFinish().push(rp);
            }
        }
        else if (x < 80)
        {
            cout << "####  RESCHEDULE A RANDOM PATIENT IN EARLY LIST  ####" << endl;

            getEarly().reschedule();
        }
        else
            cout << "#### NO ACTION  ####" << endl;

        cout << "################################################################" << endl;

        if (getFinish().getCount() == numPatients)
            ts = -1;

        ui->printAllInformation(*this, ts);

        cin.get();
    }
}