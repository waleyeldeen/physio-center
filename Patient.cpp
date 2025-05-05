#include "Patient.h"
#include "Scheduler.h"

Patient::Patient(Scheduler* s, int id, int pt, int vt, int numOfTreatments, bool isNormal)
    : s(s), id(id), pt(pt), vt(vt), numOfTreatments(numOfTreatments), isNormal(isNormal), penalty(0) {
}

Patient::Patient(const Patient* other)
{
    id = other->getId();
    pt = other->getPt();
    vt = other->getVt();
    isNormal = other->getIsNormal(); 
}

// Getters
int Patient::getId() const { return id; }
int Patient::getPt() const { return pt; }
int Patient::getVt() const { return vt; }
PatientStatus Patient::getStatus() const { return status; }
int Patient::getNumOfTreatments() const { return numOfTreatments; }
bool Patient::getIsNormal() const { return isNormal; }
int Patient::getPenalty() const { return penalty; }

// Setters
void Patient::setId(int newId) { id = newId; }
void Patient::setPt(int newPt) { pt = newPt; }
void Patient::setVt(int newVt) { vt = newVt; }
void Patient::setStatus(PatientStatus s) { status = s; }
void Patient::setPenalty(int newPenalty) { penalty = newPenalty; }


// Treatment operations

Treatment* Patient::peekReqTreatment()
{
    Treatment* toBeReturned = nullptr;
    reqTreatment.peek(toBeReturned);
    return toBeReturned;
}

bool Patient::hasTreatment(TreatmentType tt)
{
    bool exist = false;
    LinkedQueue<Treatment*> tempReqTreatment;

    Treatment* t;

    for (int i = 0; i < numOfTreatments; i++)
    {
        reqTreatment.dequeue(t);
        if (tt == t->getType())
        {
            exist = true;
        }
        tempReqTreatment.enqueue(t);
    }

    // return the reqTreatment to its original state
    for (int i = 0; i < numOfTreatments; i++)
    {
        tempReqTreatment.dequeue(t);
        reqTreatment.enqueue(t);
    }

    return exist;
}


bool Patient::addTreatment(Treatment* newT)
{
    if (reqTreatment.getCount() <= 3)
    {
        reqTreatment.enqueue(newT);
        return true;
    }
    return false;
}

bool Patient::hasLastTreatment()
{
    return reqTreatment.getCount() == 1;
}

bool Patient::reorderReqTreatment(TreatmentType tt)
{
    Treatment* t = nullptr;
    while (true)
    {
        if (reqTreatment.peek(t))
        {
            if (t->getType() != tt)
            {
                reqTreatment.dequeue(t);
                reqTreatment.enqueue(t);
            }
            else if (t->getType() == tt)
                break;
        }
    }
    return true;
}

void Patient::moveNextTreatmentToWait()
{
    Treatment* t;

    if (isNormal == true)
    {
        // patient is isNormal
        reqTreatment.peek(t);
        t->moveToWait(s);
    }
    else if (isNormal == false)
    {
        // patient is recovering
        TreatmentType arr[3];
        s->getMinLatencyArray(arr);

        // loop on arr
        for (int i = 0; i < 3; i++)
        {
            TreatmentType currMinTreatmentType = arr[i];

            // true if patient has the treatment in arr[i]
            bool treatmentExist = hasTreatment(currMinTreatmentType);

            if (treatmentExist)
            {
                reorderReqTreatment(currMinTreatmentType);
                reqTreatment.peek(t);
                t->moveToWait(s);
                break;
            }
        }
    }
}

// Output stream operator
std::ostream& operator<<(std::ostream& os, const Patient* p)
{
    os << "[Patient ID: " << p->id << ", PT: " << p->pt
        << ", VT: " << p->vt << ", Type: "
        << (p->isNormal ? "N" : "R")  << "] ";
    p->reqTreatment.print();
    return os;
}
