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
    int tt;  // total treatment time
    int wt;  // total waiting time
    int ft;  // time at which patient finished his last treatment
    bool cancel, resc;
    int assignmentTimeForWaitlist; // used for calculating WT
public:
    Patient(Scheduler* s, int id, int pt, int vt, int numOfTreatments, bool isNormal);
    Patient(const Patient* other);

    // Getters
    int getId() const;
    int getPt() const;
    int getVt() const;
    PatientStatus getStatus() const;
    int getNumOfTreatments() const;
    bool getIsNormal() const;
    int getPenalty() const;
    int getTt() const;
    int getWt() const;
    int getFt() const;
    bool getResc() const;
    bool getCancel() const;

    // Setters
    void setId(int newId);
    void setPt(int newPt);
    void setVt(int newVt);
    void setFt(int newFt);
    void setStatus(PatientStatus s);
    void setPenalty(int newPenalty);

    void resced();
    void canceled();
    void updateWt(int ts);
    void updateTt(int duration);

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
    void moveNextTreatmentToWait(int ts);

    bool finishNextTreatment();

    

    // Output stream operator
    friend std::ostream& operator<<(std::ostream& os, const Patient* p);

};
