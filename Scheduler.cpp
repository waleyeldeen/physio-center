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

void Scheduler::addToWaitU(Patient* p) { waitU.enqueue(p); }
void Scheduler::addToWaitE(Patient* p) { waitE.enqueue(p); }
void Scheduler::addToWaitX(Patient* p) { waitX.enqueue(p); }

#include "UI.h"

//void Scheduler::runSimulation(UI* ui)
//{
//    int ts = 0;
//    while (ts != -1)
//    {
//        cout << "################################################################" << endl;
//        ts++;
//        Patient* p = nullptr;
//        if (getIdle().peek(p))
//        {
//            // check if a patient has arrived
//            if (p->getVt() == ts)
//            {
//                // check if the patient is late or early
//                if (p->getPt() >= p->getVt())
//                    addToEarly();
//                else
//                    addToLate();
//            }
//        }
//
//        /*
//            Random Waiting Procedure
//        */
//        TherapyType therapy;
//        int chooseTherapy = getRandInRange(0, 100);
//        if (chooseTherapy < 33)
//            therapy = ELECTRO;
//        else if (chooseTherapy < 66)
//            therapy = ULTRA;
//        else
//            therapy = GYM;
//
//        int x = getRandInRange(0, 100);
//
//        Patient* rp = nullptr;
//        int pri;
//
//        if (x < 10)
//        {
//            cout << "####  MOVING NEXT PATIENT FROM EARLY TO RANDOMWAITING  ####" << endl;
//            // dequeue next patient from early and get pointer to it
//            if (getEarly().dequeue(rp, pri))
//            {
//                switch (therapy)
//                {
//                case ELECTRO:
//                    getWaitE().enqueue(rp); break;
//                case ULTRA:
//                    getWaitU().enqueue(rp); break;
//                case GYM:
//                    getWaitX().enqueue(rp); break;
//                }
//            }
//        }
//        else if (x < 20)
//        {
//            cout << "####  MOVING NEXT PATIENT FROM LATE TO RANDOMWAITING  ####" << endl;
//
//            // dequeue next patient from late and get pointer to it
//            if (getLate().dequeue(rp, pri))
//            {
//                int penalty = (rp->getVt() - rp->getPt()) / 2;
//                int newPt = penalty + rp->getPt();
//                rp->setPt(newPt);
//                switch (therapy)
//                {
//                case ELECTRO:
//                    getWaitE().insertSorted(rp); break;
//                case ULTRA:
//                    getWaitU().insertSorted(rp); break;
//                case GYM:
//                    getWaitX().insertSorted(rp); break;
//                }
//            }
//        }
//        else if (x < 40)
//        {
//            cout << "####  MOVING 2 NEXT PATIENTS FROM RANDOMWAITING TO SERVING  ####" << endl;
//
//            // move 2 next patients from a RandomWaiting to serving list
//            Patient* rp2 = nullptr;
//
//            bool getPatient1, getPatient2;
//
//            switch (therapy)
//            {
//            case ELECTRO:
//                getPatient1 = getWaitE().dequeue(rp);
//                getPatient2 = getWaitE().dequeue(rp2);
//                break;
//            case ULTRA:
//                getPatient1 = getWaitU().dequeue(rp);
//                getPatient2 = getWaitU().dequeue(rp2);
//                break;
//            case GYM:
//                getPatient1 = getWaitU().dequeue(rp);
//                getPatient2 = getWaitU().dequeue(rp2);
//            }
//
//            if (getPatient1)
//            {
//                // TODO: should we do a minus one to finish time or not
//                // get duration of next treatment in reqTreatment list (trivial)
//                int duration = rp->peekReqTreatment()->getDuration();
//                int finishTime = ts + duration;
//                getServing().enqueue(rp, -finishTime);
//                if (getPatient2)
//                {
//                    duration = rp2->peekReqTreatment()->getDuration();
//                    finishTime = ts + duration;
//                    getServing().enqueue(rp2, -finishTime);
//                }
//            }
//
//        }
//        else if (x < 50)
//        {
//            cout << "####  MOVING NEXT PATIENT FROM SERVING TO RANDOMWAITING  ####" << endl;
//
//            //[]TODO: should I use insertSorted or enqueue (knowing that the later breaks the PT sort in waitlist)
//            if (getServing().dequeue(rp, pri))
//            {
//                switch (therapy)
//                {
//                case ELECTRO:
//                    getWaitE().insertSorted(rp); break;
//                case ULTRA:
//                    getWaitU().insertSorted(rp); break;
//                case GYM:
//                    getWaitX().insertSorted(rp); break;
//                }
//            }
//        }
//        else if (x < 60)
//        {
//            cout << "####  MOVING NEXT PATIENT FROM SERVING TO FINISH  ####" << endl;
//
//            if (getServing().dequeue(rp, pri))
//            {
//                getFinish().push(rp);
//            }
//        }
//        else if (x < 70)
//        {
//            cout << "####  MOVING RANDOM PATIENT FROM XWAITING TO FINISH  ####" << endl;
//
//            rp = getWaitX().pickRandCancelPatient();
//            if (rp != nullptr)
//            {
//                getFinish().push(rp);
//            }
//        }
//        else if (x < 80)
//        {
//            cout << "####  RESCHEDULE A RANDOM PATIENT IN EARLY LIST  ####" << endl;
//
//            getEarly().reschedule();
//        }
//        else
//            cout << "#### NO ACTION  ####" << endl;
//
//        cout << "################################################################" << endl;
//
//        if (getFinish().getCount() == numPatients)
//            ts = -1;
//
//        ui->printAllInformation(*this, ts);
//
//        cin.get();
//    }
//}
void Scheduler::sim(UI* ui)
{
	while (true)
	{
		ts++;
		moveArrivedPatients();

		moveEarlyPatientsToWait();

		moveLatePatientsToWait();

		ui->printAllInformation(*this, ts);

		cin.get();
	}
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
			}
			else if (vt == pt)
			{
				//TODO call move to waitlist func
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

			// check if patient is normal or recovering
			bool normal = p->getType();
			if (normal == true)
				moveNormPatientToWait(p);
			else
				moveRecPatientToWait(p);

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

			// check if patient is normal or recovering
			bool normal = p->getType();
			if (normal == true)
				moveNormPatientToWait(p, true); // true is for late patients
			else
				moveRecPatientToWait(p, true); // true is for late patients

		}
		else
			break;
	}
}

void Scheduler::moveNormPatientToWait(Patient* p, bool isLate)
{
	// get type of first treatment in patient
	Treatment* currentTreatment;
	currentTreatment = p->peekReqTreatment();
	TreatmentType type = currentTreatment->getType();

	if (isLate == false)
	{
		if (type == ULTRA)
			waitU.enqueue(p);
		else if (type == ELECTRO)
			waitE.enqueue(p);
		else if (type == GYM)
			waitX.enqueue(p);
	}
	else if (isLate == true)
	{
		if (type == ULTRA)
			waitU.insertSorted(p);
		else if (type == ELECTRO)
			waitE.insertSorted(p);
		else if (type == GYM)
			waitX.insertSorted(p);
	}
}

void Scheduler::moveRecPatientToWait(Patient* p, bool isLate)
{
	//TODO: remove isLate and use insteadSorted anyway
	Treatment* currentTreatment;
	currentTreatment = p->peekReqTreatment();
	TreatmentType type = currentTreatment->getType();

	// get array of asc order latencies
	TreatmentType latArr[3];
	getMinLatencyArray(latArr);


	// loop on latArr
	for (int i = 0; i < 3; i++)
	{
		bool treatmentExist = p->hasTreatment(latArr[i]);
		// if true check which waitlist we are looking at and enqueue in it
		if (treatmentExist)
		{
			switch (latArr[i])
			{
			case ULTRA:
				waitU.insertSorted(p); break;
			case ELECTRO:
				waitE.insertSorted(p); break;
			case GYM:
				waitX.insertSorted(p); break;
			}
			// break from for loop as the priority is for the lowest latency that exists only
			break;
		}
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