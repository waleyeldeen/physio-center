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
        
        Node<Patient*>* front = this->getFrontPtr();
        int pri = newP->getPt();
        int headPri = front->getItem()->getPt();

        if (front == nullptr || pri > headPri) {

            newNode->setNext(front);
            this->setFrontPtr(newNode);
            return;
        }

        Node<Patient*>* current = front;
        while (current->getNext() && pri <= current->getNext()->getItem()->getPt()) {
            current = current->getNext();
        }
        newNode->setNext(current->getNext());
        current->setNext(newNode);
	}  

    int calcTreatmentLat() {
        return 0;
    }
};