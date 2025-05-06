#pragma once
#include <iostream>
#include "LinkedQueue.h"
#include "Treatment.h"
#include "DEFS.h"

class Scheduler;

class Patient
{
private:
    int id, pt, vt, penalty, numOfTreatments;
    PatientStatus status;
    LinkedQueue<Treatment*> reqTreatment;
    bool isNormal;  // True: Normal, False: Recovering
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
    Patient(Scheduler* s, int id, int pt, int vt, int numOfTreatments, bool isNormal);
    Patient(const Patient* other);

    // Getters
    int getId() const;
    int getPt() const;
    int getVt() const;
    PatientStatus getStatus() const;
    int getNumOfTreatments() const;
    int getIsNormal() const;
    int getPenalty() const;

    // Setters
    void setId(int newId);
    void setPt(int newPt);
    void setVt(int newVt);
    void setStatus(PatientStatus s);
    void setPenalty(int newPenalty);

    // Treatment operations
    /*
        Return a pointer to the Treatment object at the front
    */
    Treatment* peekReqTreatment();

    /*Checks if a certain treatment exists in reqTreatemnt list*/
    bool hasTreatment(TreatmentType t);

    bool addTreatment(Treatment* newT);
    bool hasLastTreatment();

    /*Reorder reqTreatment list so that the one with type tt is at the front*/
    bool reorderReqTreatment(TreatmentType tt);

    /*
        Moves patient to waiting list (N or R)
        Does not take into consideration of patient is ERLY or LATE
    */
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
