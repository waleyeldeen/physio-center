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
    bool type;  // True: Normal, False: Recovering
    Scheduler* s;

public:
    Patient(Scheduler* s, int id, int pt, int vt, int numOfTreatments, bool type);
    Patient(const Patient* other);

    // Getters
    int getId() const;
    int getPt() const;
    int getVt() const;
    PatientStatus getStatus() const;
    int getNumOfTreatments() const;
    int getType() const;
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
    bool addTreatment(Treatment* newT);
    bool hasLastTreatment();
    void moveNextTreatmentToWait();


    // Output stream operator
    friend std::ostream& operator<<(std::ostream& os, const Patient* p);
};
