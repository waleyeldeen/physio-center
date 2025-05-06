#include "Patient.h"

Patient::Patient(Scheduler* s, int id, int pt, int vt, bool type, PatientStatus st, Treatment* curr)
    : s(s), id(id), pt(pt), vt(vt), type(type), status(st), CurrentTreat(curr) {
}

Patient::Patient(const Patient* other)
{
    id = other->getId();
    pt = other->getPt();
    vt = other->getVt();
    type = other->getType();
    status = other->getStatus();
    CurrentTreat = other->getCurrentTreat();
}

// Getters
int Patient::getId() const { return id; }
int Patient::getPt() const { return pt; }
int Patient::getVt() const { return vt; }
int Patient::getType() const { return type; }
PatientStatus Patient::getStatus() const { return status; }
Treatment* Patient::getCurrentTreat() const { return CurrentTreat; }


// Setters
void Patient::setId(int newId) { id = newId; }
void Patient::setPt(int newPt) { pt = newPt; }
void Patient::setVt(int newVt) { vt = newVt; }
void Patient::setType(bool newType) { type = newType; }
void Patient::setStatus(PatientStatus newStatus) { status = newStatus; }
void Patient::setCurrentTreat(Treatment* newTreat) { CurrentTreat = newTreat; }


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


// Bonus Part
void Patient::Set_Tool_1(int x)
{
    Tool1 = Tool_1;
    Tools_Durations[0] = x;
}
void Patient::Set_Tool_2(int y)
{
    Tool2 = Tool_2;
    Tools_Durations[1] = y;
}
void Patient::Set_Tool_3(int z)
{
    Tool3 = Tool_3;
    Tools_Durations[2] = z;
}


void Patient::Unset_Tool_1()
{
    Tool1 = NON;
}
void Patient::Unset_Tool_2()
{
    Tool2 = NON;
}
void Patient::Unset_Tool_3()
{
    Tool3 = NON;
}

void Patient::Set_Active_Tool(int t)
{
    Active_Tool = t;
}
void Patient::Unset_Active_Tool()
{
    Active_Tool = 0;
}


int Patient::Get_Active_Tool()
{
    return Active_Tool;
}

Tools Patient::GetTool_1()
{
    return Tool1;
}
Tools Patient::GetTool_2()
{
    return Tool2;
}
Tools Patient::GetTool_3()
{
    return Tool3;
}

int Patient::Get_Duration_1()
{
    return Tools_Durations[0];
}
int Patient::Get_Duration_2()
{
    return Tools_Durations[1];
}
int Patient::Get_Duration_3()
{
    return Tools_Durations[2];
}