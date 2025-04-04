#pragma once
#include <iostream>
#include "LinkedQueue.h"
#include "Treatment.h"
#include "DEFS.h"

class Patient
{
private:
    int id, pt, vt;
    PatientStatus status;
    LinkedQueue<Treatment*> reqTreatment;
    bool type;  // True: Normal, False: Recovering

public:
    Patient();
    Patient(int id, int pt, int vt, bool type);
    Patient(const Patient* other);

    // Getters
    int getId() const;
    int getPt() const;
    int getVt() const;
    PatientStatus getStatus() const;
    int getType() const;

    // Setters
    void setId(int newId);
    void setPt(int newPt);
    void setVt(int newVt);
    void setStatus(PatientStatus s);

    // Treatment operations
    /*
        Return a pointer to the Treatment object at the front
    */
    Treatment* peekReqTreatment();
    bool addTreatment(Treatment* newT);
    bool hasLastTreatment();

    // Output stream operator
    friend std::ostream& operator<<(std::ostream& os, const Patient* p);
};
