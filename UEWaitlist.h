#pragma once  
#include "LinkedQueue.h"  
#include "Patient.h"  
using namespace std;  

class UEWaitlist : public LinkedQueue<Patient*>  
{  
public:
	void insertSorted(Patient* const newP)  
	{  
        Node<Patient*>* newNode = new Node<Patient*>(newP);
        
        Node<Patient*>* front = frontPtr;
        int pri = newP->getPt();
        int headPri = front->getItem()->getPt();

        if (front == nullptr || pri > headPri) {

            newNode->setNext(front);
            frontPtr = newNode;
            return;
        }

        Node<Patient*>* current = front;
        while (current->getNext() && pri <= current->getNext()->getItem()->getPt()) {
            current = current->getNext();
        }
        newNode->setNext(current->getNext());
        current->setNext(newNode);
	}  

    /*
    Returns the sum of all treatment durations in waiting list.
    used for recovering patient
    Assume that the treatment at the front of the reqTreatment Queue is of the same
    type the patient is currently waiting in.
    */
    int calcTreatmentLat()
    {
        int latency = 0;
        Node<Patient*>* current = frontPtr;

        for (int i = 0; i < this->count; i++)
        {
            //                  Node*    Patient*    Treatment*          int
            latency = latency + current->getItem()->peekReqTreatment()->getDuration();
        }
    }
};