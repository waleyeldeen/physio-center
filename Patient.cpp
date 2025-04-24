#include "Patient.h"

Patient::Patient(Scheduler* s, int id, int pt, int vt, bool type)
    : s(s), id(id), pt(pt), vt(vt), type(type) {
}

Patient::Patient(const Patient* other)
{
    id = other->getId();
    pt = other->getPt();
    vt = other->getVt();
    type = other->getType();
    status = other->getStatus();
}

// Getters
int Patient::getId() const { return id; }
int Patient::getPt() const { return pt; }
int Patient::getVt() const { return vt; }
int Patient::getType() const { return type; }
PatientStatus Patient::getStatus() const { return status; }


// Setters
void Patient::setId(int newId) { id = newId; }
void Patient::setPt(int newPt) { pt = newPt; }
void Patient::setVt(int newVt) { vt = newVt; }
void Patient::setType(bool newType) { type = newType; }
void Patient::setStatus(PatientStatus newStatus) { status = newStatus; }


// Treatment operations

Treatment* Patient::peekReqTreatment()
{
    Treatment* toBeReturned = nullptr;
    reqTreatment.peek(toBeReturned);
    return toBeReturned;
}


bool Patient::addTreatment(Treatment* newT)
{
    if (reqTreatment.getCount() < 3)
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
        << (p->type ? "Normal]" : "Recovering]");
    return os;
}
