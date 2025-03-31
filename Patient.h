#pragma once
using namespace std;
#include <iostream>
#include "LinkedQueue.h"
#include "Treatment.h"

class Patient
{
private:
	int id, pt, vt;

	// TODO: Ask Eng if this is allowed or not
	LinkedQueue<Treatment> reqTreatment;  // make reqTreatment made of Treatment obj instead of pointer

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
	
	// returns true if treatment is added succeffuly
	// returns false if treatment cannot be added (max no of treatments)
	bool addTreatment(Treatment newT)
	{
		if (reqTreatment.getCount() <= 3)
		{
			reqTreatment.enqueue(newT);
			return true;
		}
		else return false;
	}

	// check if a patient has anymore treatments in the reqTreatment list
	// used by cancel function in XWaitlist
	// TODO: create a function that checks if patient has any treatments after.
	bool hasLastTreatment() {
		if (reqTreatment.getCount() == 1) return true;
		return false;
	}

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