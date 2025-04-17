#include <iostream>
using namespace std;
#include "Scheduler.h"
#include "UI.h"

int main()
{
    srand(time(0));
    Scheduler s;
    UI* ui = new UI();

    string name = ui->inInputFileName();
    s.loadInputFile(name);

    // Call the new function in Scheduler
}