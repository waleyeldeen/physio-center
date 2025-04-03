#include "UI.h"

UI::UI(bool isIneractive) : isInteractive(isIneractive) {}

string UI::inInputFileName()
{
	string name;
	cout << "Enter input file name (default: input.txt): ";
	char first = cin.peek();

	if (first == '\n') {
		cin.get(); // consume the newline
		return "input.txt";
	}
	else {
		cin >> name;
		return name;
	}
}

void UI::printAllInformation(Scheduler& sObj, int ts)
{
	cout << "Current Timestep: " << ts << endl
		<< "=================  ALL List  =================" << endl
		<< sObj.getIdle().getCount() << " patients remaining: " << endl;
	// print idle
	sObj.getIdle().print(true);
}

