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
	int* numTools_1, * numTools_2, * numTools_3;
	numTools_1 = new int[numXRooms];
	numTools_2 = new int[numXRooms];
	numTools_3 = new int[numXRooms];
	for (int i = 0; i < numXRooms; i++) 
	{
		file >> xCap[i];
		file >> numTools_1[i];
		file >> numTools_2[i];
		file >> numTools_3[i];
	}



	// add device objects
	// Electro Devices
	for (int i = 0; i < numEDevices; i++)
	{
		// for the sake of the code I will make the id of each resource the i*10
		eDevices.enqueue(new EDevice((i + 1) * 10, EDEVICE));
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
		XRoom* x = new XRoom((i + 1) * 1000, XROOM, xCap[i]);
		for (int j = 0; j < numTools_1[i]; j++)
		{
			x->increment_Tool_1();
		}
		for (int k = 0; k < numTools_1[i]; k++)
		{
			x->increment_Tool_2();
		}
		for (int o = 0; o < numTools_1[i]; o++)
		{
			x->increment_Tool_3();
		}
		if (xCap[i] != 0 && x->GetSumOfTools() != 0)
			xRooms.enqueue(x);

		// Assume that the capacity can never be larger than the sum of all tools in the room (Not Logical)
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
		             int NumOfRequiredTools;
		             file >> NumOfRequiredTools;
		             int* ToolType = new int[NumOfRequiredTools];
		             int* Durations = new int[NumOfRequiredTools];
		             for (int i = 0; i < NumOfRequiredTools; i++)
		             {
			              file >> ToolType[i];
			              file >> Durations[i];
			              switch (ToolType[i])
			              {
			               case '1':
				             newP->Set_Tool_1(Durations[i]); break;
			               case '2':
				             newP->Set_Tool_2(Durations[i]); break;
			               case '3':
				             newP->Set_Tool_3(Durations[i]); break;
			              }
			                // Sum of Durations Array Elements should be equal to the value "duration"
		             }
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
}

void Scheduler::sim(UI* ui)
{
	while (true)
	{
		ts++;
		moveArrivedPatients();

		moveEarlyPatientsToWait();

		moveLatePatientsToWait();

		moveUWaitPatientsToServe();
		moveEWaitPatientsToServe();
		moveXWaitPatientsToServe();

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
				p->setStatus(ERLY);
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

			p->moveNextTreatmentToWait();
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

			p->moveNextTreatmentToWait();
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
	Patient* PT;
	PriQueue<Patient*> TEMP;
	int Khairy;
	while (this->getServing().dequeue(PT, Khairy))
	{

		if (PT->peekReqTreatment()->Get_X_Room() != nullptr)
		{

			if ((PT->peekReqTreatment()->getAssignmentTime() + PT->peekReqTreatment()->getDuration()) == ts)
			{
				XRoom* X = PT->peekReqTreatment()->Get_X_Room();

				Resource* R = new XRoom();
				PT->peekReqTreatment()->setAssignedRes(R);
				PT->peekReqTreatment()->Set_X_Room(nullptr);

				X->decrementNumOfPTsIn();
				if ((X->getNumOfPts() + 1) == X->getCapacity())
					this->getXRooms().enqueue(X);

				PT->peekReqTreatment()->setAssignmentTime(0);
				PT->peekReqTreatment()->setDuration(0);


				int Active = PT->Get_Active_Tool();
				PT->Set_Active_Tool(0);

				if (Active == 1)
				{
					X->increment_Tool_1();
				}
				else if (Active == 2)
				{
					X->increment_Tool_2();
				}
				else if (Active == 3)
				{
					X->increment_Tool_3();
				}

				if (PT->GetTool_1() == NON && PT->GetTool_2() == NON && PT->GetTool_3() == NON)
					this->getFinish().push(PT);
				else
					this->addToWaitX(PT);
			}
			else
				TEMP.enqueue(PT, Khairy);
		}
		else
			TEMP.enqueue(PT, Khairy);
	}

	while (TEMP.dequeue(PT, Khairy))
		this->getServing().enqueue(PT, Khairy);

	XTherapy* XTh = new XTherapy();
	XRoom* XX;
	Patient* PP;

	LinkedQueue<XRoom*> Temporary;
	XWaitlist temp;
	while (this->getXRooms().dequeue(XX))
	{

		while (XX->GetNumOfTool_1() != 0 && this->getWaitX().getCount() != 0)
		{
			while (this->getWaitX().dequeue(PP))
			{
				if (PP->GetTool_1() == Tool_1 && PP->Get_Active_Tool() == 0)
				{

					XX->incrementNumOfPts();
					XX->decrement_Tool_1();
					PP->peekReqTreatment()->setAssignmentTime(ts);
					PP->peekReqTreatment()->setAssignedRes(XX);
					PP->peekReqTreatment()->Set_X_Room(XX);
					PP->peekReqTreatment()->setDuration(PP->Get_Duration_1());
					PP->Unset_Tool_1();
					PP->Set_Active_Tool(1);
					this->addToServe(PP);

				}
				else
					temp.enqueue(PP);
			}
		}
		if (XX->getNumOfPts() != XX->getCapacity() && XX->GetSumOfTools() != 0)
			Temporary.enqueue(XX);
	}

	while (Temporary.dequeue(XX))
		this->getXRooms().enqueue(XX);

	while (temp.dequeue(PP))
		this->getWaitX().enqueue(PP);


	while (this->getXRooms().dequeue(XX))
	{

		while (XX->GetNumOfTool_2() != 0 && this->getWaitX().getCount() != 0)
		{
			while (this->getWaitX().dequeue(PP))
			{
				if (PP->GetTool_2() == Tool_2 && PP->Get_Active_Tool() == 0)
				{

					XX->incrementNumOfPts();
					XX->decrement_Tool_2();
					PP->peekReqTreatment()->setAssignmentTime(ts);
					PP->peekReqTreatment()->setAssignedRes(XX);
					PP->peekReqTreatment()->Set_X_Room(XX);
					PP->peekReqTreatment()->setDuration(PP->Get_Duration_2());
					PP->Unset_Tool_2();
					PP->Set_Active_Tool(2);
					this->addToServe(PP);

				}
				else
					temp.enqueue(PP);
			}
		}
		if (XX->getNumOfPts() != XX->getCapacity() && XX->GetSumOfTools() != 0)
			Temporary.enqueue(XX);
	}

	while (Temporary.dequeue(XX))
		this->getXRooms().enqueue(XX);

	while (temp.dequeue(PP))
		this->getWaitX().enqueue(PP);


	while (this->getXRooms().dequeue(XX))
	{

		while (XX->GetNumOfTool_3() != 0 && this->getWaitX().getCount() != 0)
		{
			while (this->getWaitX().dequeue(PP))
			{
				if (PP->GetTool_3() == Tool_3 && PP->Get_Active_Tool() == 0)
				{

					XX->incrementNumOfPts();
					XX->decrement_Tool_3();
					PP->peekReqTreatment()->setAssignmentTime(ts);
					PP->peekReqTreatment()->setAssignedRes(XX);
					PP->peekReqTreatment()->Set_X_Room(XX);
					PP->peekReqTreatment()->setDuration(PP->Get_Duration_3());
					PP->Unset_Tool_3();
					PP->Set_Active_Tool(3);
					this->addToServe(PP);

				}
				else
					temp.enqueue(PP);
			}
		}
		if (XX->getNumOfPts() != XX->getCapacity() && XX->GetSumOfTools() != 0)
			Temporary.enqueue(XX);
	}

	while (Temporary.dequeue(XX))
		this->getXRooms().enqueue(XX);

	while (temp.dequeue(PP))
		this->getWaitX().enqueue(PP);


}