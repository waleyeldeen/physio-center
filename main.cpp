#include <iostream>
using namespace std;
#include "Scheduler.h"
#include "UI.h"

#include "ETherapy.h"
#include "UTherapy.h"
#include "XTherapy.h"

int main()
{
    //srand(time(0));
   // Scheduler s;
   // UI* ui = new UI();

    Scheduler* ss = new Scheduler();
    Treatment * tt = nullptr;
    Patient* p1 = new Patient(ss, 1, 0, 0, true, IDLE, tt);
    ss->addToWaitE(p1);
    Patient* p2 = new Patient(ss, 2, 0, 0, true, IDLE, tt);
    ss->addToWaitU(p2);
    Patient* p3 = new Patient(ss, 3, 0, 0, true, IDLE, tt);
    ss->addToWaitX(p3);
    EDevice* E = new EDevice();
    UDevice* U = new UDevice();
    XRoom* X = new XRoom();
    X->setCapacity(2);
    ss->getEDevices().enqueue(E);
    ss->getUDevices().enqueue(U);
    ss->getXRooms().enqueue(X);

    ETherapy* e = new ETherapy();
    p1->addTreatment(e);
    UTherapy* u = new UTherapy();
    p2->addTreatment(u);
    XTherapy* x = new XTherapy();
    p3->addTreatment(x);

    cout << ss->getWaitE().getCount() << "  ";
    cout << ss->getWaitU().getCount() << "  ";
    cout << ss->getWaitX().getCount() << endl;

    cout << ss->getEDevices().getCount() << "  ";
    cout << ss->getUDevices().getCount() << "  ";
    cout << ss->getXRooms().getCount() << endl;



    cout << endl;



    
    ss->Move_EWaitngPTs_to_Serve();
    

    
    ss->Move_UWaitingPTs_to_Serve();
    


    ss->Move_XWaitingPTs_to_Serve();
    

    cout << endl;


    cout << ss->getWaitE().getCount() << "  ";
    cout << ss->getWaitU().getCount() << "  ";
    cout << ss->getWaitX().getCount() << endl;

    cout << ss->getEDevices().getCount() << "  ";
    cout << ss->getUDevices().getCount() << "  ";
    cout << ss->getXRooms().getCount() << endl;

    ss->IncrementTimeStep();
    cout << endl;

    Patient* p4 = new Patient(ss, 4, 0, 0, true, IDLE, tt);
    ss->addToWaitX(p4);

    
    XTherapy* x1 = new XTherapy();
    p4->addTreatment(x1);


    ss->Move_EWaitngPTs_to_Serve();



    ss->Move_UWaitingPTs_to_Serve();



    ss->Move_XWaitingPTs_to_Serve();


    cout << ss->getWaitE().getCount() << "  ";
    cout << ss->getWaitU().getCount() << "  ";
    cout << ss->getWaitX().getCount() << endl;

    cout << ss->getEDevices().getCount() << "  ";
    cout << ss->getUDevices().getCount() << "  ";
    cout << ss->getXRooms().getCount() << endl;

    cout << endl;

    // Final Test
    XRoom* XX = new XRoom();
    XX->setCapacity(5);
    ss->getXRooms().enqueue(XX);
    for (int i = 0; i < 5; i++)
    {
        Patient* p1 = new Patient(ss, i+11, 0, 0, true, IDLE, tt);
        ss->addToWaitE(p1);
        Patient* p2 = new Patient(ss, i+12, 0, 0, true, IDLE, tt);
        ss->addToWaitU(p2);
        Patient* p3 = new Patient(ss, i+13, 0, 0, true, IDLE, tt);
        ss->addToWaitX(p3);
        EDevice* E = new EDevice();
        UDevice* U = new UDevice();
       
        ss->getEDevices().enqueue(E);
        ss->getUDevices().enqueue(U);
        

        ETherapy* e = new ETherapy();
        p1->addTreatment(e);
        UTherapy* u = new UTherapy();
        p2->addTreatment(u);
        XTherapy* x = new XTherapy();
        p3->addTreatment(x);


        cout << ss->getWaitE().getCount() << "  ";
        cout << ss->getWaitU().getCount() << "  ";
        cout << ss->getWaitX().getCount() << endl;

        cout << ss->getEDevices().getCount() << "  ";
        cout << ss->getUDevices().getCount() << "  ";
        cout << ss->getXRooms().getCount() << endl;


        cout << endl;



        ss->Move_EWaitngPTs_to_Serve();



        ss->Move_UWaitingPTs_to_Serve();



        ss->Move_XWaitingPTs_to_Serve();


        cout << endl;

        cout << ss->getWaitE().getCount() << "  ";
        cout << ss->getWaitU().getCount() << "  ";
        cout << ss->getWaitX().getCount() << endl;

        cout << ss->getEDevices().getCount() << "  ";
        cout << ss->getUDevices().getCount() << "  ";
        cout << ss->getXRooms().getCount() << endl;

        ss->IncrementTimeStep();
        cout << endl;
        int z;
        cin >> z;
    }

   // string name = ui->inInputFileName();
   // s.loadInputFile(name);

    // Call the new function in Scheduler
   // s.runSimulation(ui);

    return 0;
}