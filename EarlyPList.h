#pragma once
using namespace std;

#include "PriQueue.h"
#include "Patient.h"
#include "Helpers.h"


class EarlyPList : public PriQueue<Patient*> // int is temporary unit patient class is created
{
public:
	/* 
		This function should pick a random patient from the Priority Queue
		and change  the PT to random one in the future.
		To fix the position of the patient in the Priority Queue, dequeue and enqueue the patient again
		TODO: Make it return true if rescheduled and false if list is empty
	*/
	bool reschedule(Patient* p)
	{
		if (this->isEmpty()) { return false; }
		// the -1 is to avoid being close to the nullptr (end of queue)
		int randomIteration = getRandInRange(0, this->getCount() - 1);
		PriNode<Patient*>* current = nullptr;
		PriNode<Patient*>* previous = nullptr;
		Patient* selectedPatient = nullptr;

		int pri;

		current = head;

		// pick the random patient
		for (int i = 0; i < randomIteration; i++)
		{
			previous = current;
			current = current->getNext();
		}

		// remove node from queue incase we want to remove patient at front of queue
		if (current == head)
		{
			this->dequeue(selectedPatient, pri);

		}
		// node is in the middle or at the end
		else
		{
			selectedPatient = current->getItem(pri);

			previous->setNext(current->getNext());
			count--;
		}
		
		int newPt = selectedPatient->getPt() + getRandInRange(0, selectedPatient->getPt()/2);
		selectedPatient->setPt(newPt);

		this->enqueue(selectedPatient, -newPt);
		p = selectedPatient;
		return true;
	}
};