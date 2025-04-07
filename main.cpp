#include <iostream>
using namespace std;
#include "Scheduler.h"
#include "UI.h"

int main()
{
    srand(time(0));
    Scheduler s;

    UI ui;

    string name = ui.inInputFileName();
    s.loadInputFile(name);
    
    int ts = 0;
    // start of simulation
    while (ts != -1)
    {
        cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
        ts++;
        Patient* p = nullptr;
        if (s.getIdle().peek(p))
        {
            // check if a patient has arrived
            if (p->getVt() == ts)
            {
                // check if the patient is late or early
                if (p->getPt() >= p->getVt())
                    s.addToEarly();
                else
                    s.addToLate();
            }
        }

		/*
	        Random Waiting Procdure
        */
        TherapyType therapy;
        int chooseTherapy = getRandInRange(0, 100);
        if (chooseTherapy < 33)
	        therapy = ELECTRO;
        else if (chooseTherapy < 66)
	        therapy = ULTRA;
        else
	        therapy = GYM;

        int x = getRandInRange(0, 105);
        cout << "####RANDOM: " << x << endl;

	    Patient* rp;
	    int pri;

        if (x < 15)
        {
	        // dequeue next patient from early and get pointer to it
	        if (s.getEarly().dequeue(rp, pri))	// this line should be transferred
	        {
		        switch (therapy)
		        {
		        case ELECTRO:
			        s.getWaitE().enqueue(rp); break;
		        case ULTRA:
			        s.getWaitU().enqueue(rp); break;
		        case GYM:
			        s.getWaitX().enqueue(rp); break;
		        }
	        }
        }
        else if (x < 30)
        {
            // dequeu next patient from late and get point to it
            if (s.getLate().dequeue(rp, pri))
            {
                int penalty = (rp->getVt() - rp->getPt()) / 2;
                int newPt = penalty + rp->getPt();
                rp->setPt(newPt);
                switch (therapy)
                {
                case ELECTRO:
                    s.getWaitE().insertSorted(rp); break;
                case ULTRA:
                    s.getWaitU().insertSorted(rp); break;
                case GYM:
                    s.getWaitX().insertSorted(rp); break;
                }
            }
        }
        else if (x < 45)
        {
            // more 2 next patient from a RandomWaiting to serving list
            Patient* rp2;

            bool getPatient1, getPatient2;
            
            switch (therapy)
            {
            case ELECTRO:
                getPatient1 = s.getWaitE().dequeue(rp);
                getPatient2 = s.getWaitE().dequeue(rp2);
                break;
            case ULTRA:
                getPatient1 = s.getWaitU().dequeue(rp);
                getPatient2 = s.getWaitU().dequeue(rp2);
                break;
            case GYM:
                getPatient1 = s.getWaitU().dequeue(rp);
                getPatient2 = s.getWaitU().dequeue(rp2);
            }

            if (getPatient1)
            {
                //TODO: shoud it be negetive or what???
                int finishTime = ts + 10 - 1;
                s.getServing().enqueue(rp, finishTime);
                if (getPatient2)
                {
                    s.getServing().enqueue(rp2, finishTime);
                }
            }

        }
        else if (x < 60)
        {
            if (s.getServing().dequeue(rp, pri))
            {
                switch (therapy)
                {
                case ELECTRO:
                    s.getWaitE().enqueue(rp); break;
                case ULTRA:
                    s.getWaitU().enqueue(rp); break;
                case GYM:
                    s.getWaitX().enqueue(rp); break;
                }
            }
        }
        else if (x < 75)
        {
            if (s.getServing().dequeue(rp, pri))
            {
                s.getFinish().push(rp);
            }
        }
        else if (x < 90)
        {
            rp = s.getWaitX().pickRandCancelPatient();
            if (rp != nullptr)
            {
                s.getFinish().push(rp);
            }
        }
        else
        {
            s.getEarly().reschedule();
        }


        if (s.getFinish().getCount() == 10)
            ts = -1;
        
        ui.printAllInformation(s, ts);

        cin.get();
    }
}