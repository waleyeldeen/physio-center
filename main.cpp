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

		if (s.getIdle().getCount() == 0)
			ts = -1;
		
		ui.printAllInformation(s, ts);
		int x;
		cin.get();
	}
}