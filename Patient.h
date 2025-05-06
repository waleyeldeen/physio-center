#pragma once
#include <iostream>
#include "LinkedQueue.h"
#include "Treatment.h"
#include "DEFS.h"

class Scheduler;

class Patient
{
private:


    int id, pt, vt;
    bool type;  // True: Normal, False: Recovering
    PatientStatus status;
    Treatment* CurrentTreat;
    LinkedQueue<Treatment*> reqTreatment;
    Scheduler* s;
    Tools Tool1 = NON;
    Tools Tool2 = NON;
    Tools Tool3 = NON;
    int Tools_Durations[3] = { 0,0,0 }; 
    // index 0 represents the duration of tool 1 
    // index 1 represents the duration of tool 2 
    // index 2 represents the duration of tool 3

    int Active_Tool = 0; // Indicates the current tool that this patient is currently using
    

public:
    Patient(Scheduler* s, int id, int pt, int vt, bool type, PatientStatus st, Treatment* curr);
    Patient(const Patient* other);

    // Getters
    int getId() const;
    int getPt() const;
    int getVt() const;
    int getType() const;
    PatientStatus getStatus() const;
    Treatment* getCurrentTreat() const;
    

    // Setters
    void setId(int newId);
    void setPt(int newPt);
    void setVt(int newVt);
    void setType(bool newType);
    void setStatus(PatientStatus newStatus);
    void setCurrentTreat(Treatment* newTreat);

    // Treatment operations
    /*
        Return a pointer to the Treatment object at the front
    */
    Treatment* peekReqTreatment();

    bool addTreatment(Treatment* newT);
    bool hasLastTreatment();
    void moveNextTreatmentToWait();

    // Output stream operator
    friend std::ostream& operator<<(std::ostream& os, const Patient* p);

    //Bonus Part
    void Set_Tool_1(int x);
    void Set_Tool_2(int y);
    void Set_Tool_3(int z);


    void Unset_Tool_1();
    void Unset_Tool_2();
    void Unset_Tool_3();

    void Set_Active_Tool(int t);
    void Unset_Active_Tool();

    int Get_Active_Tool();

    Tools GetTool_1();
    Tools GetTool_2();
    Tools GetTool_3();



    int Get_Duration_1();
    int Get_Duration_2();
    int Get_Duration_3();

    
};
