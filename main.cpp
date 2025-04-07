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
        ts++;
        Patient* p = nullptr;
        s.getIdle().peek(p);

        // check if a patient has arrived
        if (p->getVt() == ts)
        {
            // check if the patient is late or early
            if (p->getPt() >= p->getVt())
                s.addToEarly();
            else
                s.addToLate();
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

        int x = getRandInRange(0, 100);

	    Patient* rp;
	    int pri;

        if (x < 50)
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
        else if (x < 100)
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


        if (s.getIdle().getCount() == 0)
            ts = -1;
        
        ui.printAllInformation(s, ts);

        cin.get();
    }
}