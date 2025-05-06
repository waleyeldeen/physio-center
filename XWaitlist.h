#pragma once
#include "UEWaitlist.h"
#include "Helpers.h"
using namespace std;


class XWaitlist : public UEWaitlist
{
public:
	/*
		function that removes a patient from the queue and return a pointer to it
		there is a probability Pcancel loaded from the input file
		a number X is generated at each timestep from 0 to 100, if X is less than
		Pcancel then a pick a rancom patient from the waitlist and cancel his
		treatment (move to finish)
		Conditions:
		Patient has to have XTherapy as his last treatment
		Note that the function can only be called on xWaitlist, so the current treatment will always
		be gym (presumably).
	*/
	bool pickRandCancelPatient(Patient* p)
	{
		// []TODO: what can I do if the list is empty
		if (this->isEmpty()) { return false; }


		Node<Patient*>* current = nullptr;
		Node<Patient*>* previous = nullptr;
		Patient* patientPtr = nullptr;
		bool isLastTreatment = false;
		int randomIteration;

		// generate a number between 0 and number of patient in waitlist
		// the minus 1 is because we already started at frontPtr
		// I could remove the minus one but make i = 1 initially
		randomIteration = getRandInRange(0, this->getCount() - 1);

		// pick the random patient and check if he has xTherapy as his last treatment
		// we have to traverse through the nodes to reach a pointer to the patient we want to cancel
		// incase randomIteration = 0, then for loop won't be executed making the current point to front
		current = frontPtr;
		for (int i = 0; i < randomIteration; i++)
		{
			previous = current;
			current = current->getNext();
		}

		isLastTreatment = current->getItem()->hasLastTreatment();
		p = current->getItem();

		// if XTherapy is not the last treatment 
		if (!isLastTreatment)
			return false;

		// remove node from queue
		// incase we want to remove patient at front of queue
		if (current == frontPtr)
		{
			// incase of node is the at the front
			this->dequeue(patientPtr);
		}
		else
		{
			// incase of node is in the middle or at the end
			patientPtr = current->getItem();

			// make the previous point at the next of the current
			previous->setNext(current->getNext());
			if (current == backPtr) // If the node is the last node
			{
				backPtr = previous;
			}
			count--;
		}

		return true;
	}
};