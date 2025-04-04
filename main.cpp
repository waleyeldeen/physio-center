#include <iostream>
using namespace std;
#include "Scheduler.h"
#include "UI.h"

int main()
{
	Scheduler test;

	UI ui;

	string name = ui.inInputFileName();
	test.loadInputFile(name);
	
	ui.printAllInformation(test, 1);
}