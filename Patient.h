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
    

public:
    Patient(Scheduler* s, int id, int pt, int vt, bool type);
    Patient(const Patient* other);

    // Getters
    int getId() const;
    int getPt() const;
    int getVt() const;
    int getType() const;
    PatientStatus getStatus() const;
    

    // Setters
    void setId(int newId);
    void setPt(int newPt);
    void setVt(int newVt);
    void setType(bool newType);
    void setStatus(PatientStatus newStatus);

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
};
