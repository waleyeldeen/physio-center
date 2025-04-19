#include "Patient.h"

Patient::Patient(Scheduler* s, int id, int pt, int vt, int numOfTreatments, bool type)
    : s(s), id(id), pt(pt), vt(vt), numOfTreatments(numOfTreatments), type(type), penalty(0) {
}

Patient::Patient(const Patient* other)
{
    id = other->getId();
    pt = other->getPt();
    vt = other->getVt();
    type = other->getType(); 
}

// Getters
int Patient::getId() const { return id; }
int Patient::getPt() const { return pt; }
int Patient::getVt() const { return vt; }
PatientStatus Patient::getStatus() const { return status; }
int Patient::getNumOfTreatments() const { return numOfTreatments; }
int Patient::getType() const { return type; }
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

void Patient::moveNextTreatmentToWait()
{
    Treatment* t;
    reqTreatment.peek(t);
    t->moveToWait(s);
}

// Output stream operator
std::ostream& operator<<(std::ostream& os, const Patient* p)
{
    os << "[Patient ID: " << p->id << ", PT: " << p->pt
        << ", VT: " << p->vt << ", Type: "
        << (p->type ? "N]" : "R]");

    return os;
}
