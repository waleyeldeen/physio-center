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
		fix: what if empty
	*/
	void reschedule()
	{
		if (this->isEmpty()) { return; }
		// the -1 is to avoid being close to the nullptr (end of queue)
		int randomIteration = getRandInRange(0, this->getCount() - 1);
		PriNode<Patient*>* current = this->getHead();

		Patient* copy;
		int pri;

		// pick the random patient
		if (randomIteration == 0)
		{
			this->setHead(current->getNext());
			copy = current->getItem(pri);
		}
		else
		{
			for (int i = 0; i < randomIteration - 1; i++)
			{
				current = current->getNext();
			}

			// remove what is after current
			PriNode<Patient*>* toBeDeleted = current->getNext();
			current->setNext(toBeDeleted->getNext());
			copy = toBeDeleted->getItem(pri);
		}
		
		
		int newPt = copy->getPt() + getRandInRange(0, pri/2);
		copy->setPt(newPt);

		this->enqueue(copy, newPt);
		this->decCount();
	}
};