#include "UI.h"

UI::UI(bool isInteractive) : isInteractive(isInteractive) {}
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

    cout << "=================  Waiting Lists  =================" << endl
        << sObj.getWaitE().getCount() << " Electro Therapy Patients: " << endl;
    sObj.getWaitE().print(true);
    cout << sObj.getWaitU().getCount() << " Ultra Therapy Patients: " << endl;
    sObj.getWaitU().print(true);
    cout << sObj.getWaitX().getCount() << " X Therapy Patients: " << endl;
    sObj.getWaitX().print(true);

    cout << endl;

    cout << "=================  Early List  =================" << endl
        << sObj.getEarly().getCount() << " patients: " << endl;
    sObj.getEarly().print(true);

    cout << endl;

    cout << "=================  Late List  =================" << endl
        << sObj.getLate().getCount() << " patients: " << endl;
    sObj.getLate().print(true);

    cout << endl;

    //cout << "=================  Avail E-Devices  =================" << endl
    //    << sObj.getEDevices().getCount() << " Electro Devices: ";
    //sObj.getEDevices().print();

    //cout << endl;

    //cout << "=================  Avail U-Devices  =================" << endl
    //    << sObj.getUDevices().getCount() << " Ultra Devices: ";
    //sObj.getUDevices().print();

    //cout << endl;

    //cout << "=================  Avail X-Rooms  =================" << endl
    //    << sObj.getXRooms().getCount() << " Rooms: ";
    //sObj.getXRooms().print();

    //cout << endl;

    cout << "=================  Serving List  =================" << endl
        << sObj.getServing().getCount() << endl;
    sObj.getServing().print(true);

    cout << "=================  Finish List  =================" << endl
        << sObj.getFinish().getCount() << endl;
    sObj.getFinish().print(true);

}

