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
	*/
	Patient* pickRandCancelPatient()
	{
		// TODO: what can I do if the list is empty
		// if (this->isEmpty()) { return; }

		Node<Patient*>* current = nullptr;
		Node<Patient*>* previous = nullptr;
		Patient* patientPtr = nullptr;
		bool isLastTreatment = false;
		int randomIteration;

		// pick the random patient
		while (!isLastTreatment)
		{
			current = frontPtr;

			randomIteration = getRandInRange(0, this->getCount() - 1);

			if (randomIteration != 0)
			{
				for (int i = 0; i < randomIteration; i++)
				{
					previous = current;
					current = current->getNext();
				}

				isLastTreatment = current->getItem()->hasLastTreatment();
			}
		}

		// remove node from queue
		if (randomIteration == 0)
		{
			// incase of node is the at the front
			this->dequeue(patientPtr);
		}
		else
		{
			// incase of node is in the middle or at the end

		}

		return current->getItem();

	}
};