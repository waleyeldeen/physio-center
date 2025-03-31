#pragma once
using namespace std;
#include <iostream>

class Patient
{
private:
	int id, pt, vt;

	// True: Normal Patient
	// False: Recovering Patient
	bool type;

public:
	Patient() : id(0), pt(0), vt(0), type(true) {}
	Patient(int id, int pt, int vt, bool type) : id(id), pt(pt), vt(vt), type(type) {}

	// getters
	int getId() const { return id; }
	int getPt() const { return pt; }
	int getVt() const { return vt; }
	int getType() const { return type; }

	// setters
	void setId(int newId) { id = newId; }
	void setPt(int newPt) { pt = newPt; }
	void setVt(int newVt) { vt = newVt; }

	// friend function to overload << operator
	friend ostream& operator<<(ostream& os, const Patient* p) {
		os << "[Patient ID: " << p->id << ", PT: " << p->pt << ", VT: " << p->vt << ", Type: " << (p->type ? "Normal]" : "Recovering]");
		return os;
	}

	// copy constructor
	Patient(const Patient* other)
	{
		id = other->getId();
		pt = other->getPt();
		vt = other->getVt();
		type = other->getType();
	}
};