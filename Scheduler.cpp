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
    int *numTools_1, *numTools_2, *numTools_3;
    numTools_1 = new int[numXRooms];
    numTools_2 = new int[numXRooms];
    numTools_3 = new int[numXRooms];
    for (int i = 0; i < numXRooms; i++) {
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
        if(xCap[i] != 0 && x->GetSumOfTools() != 0)
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
                if((X->getCounter() + 1) == X->getCapacity())
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

                        XX->incrementNumOfPTsIn();
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
            if (XX->getCounter() != XX->getCapacity() && XX->GetSumOfTools() != 0)
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

                        XX->incrementNumOfPTsIn();
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
            if (XX->getCounter() != XX->getCapacity() && XX->GetSumOfTools() != 0)
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

                        XX->incrementNumOfPTsIn();
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
            if (XX->getCounter() != XX->getCapacity() && XX->GetSumOfTools() != 0)
                Temporary.enqueue(XX);
        }

        while (Temporary.dequeue(XX))
            this->getXRooms().enqueue(XX);

        while (temp.dequeue(PP))
            this->getWaitX().enqueue(PP);
    

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