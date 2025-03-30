#pragma once
#include "PriNode.h"
#include <iostream>
using namespace std;

//This class impelements the priority queue as a sorted list (Linked List)
//The item with highest priority is at the front of the queue
template <typename T>
class PriQueue
{
    PriNode<T>* head;
    int count;
public:
    PriQueue() : head(nullptr), count(0) {}

    ~PriQueue() {
        T tmp;
        int p;
        while (dequeue(tmp, p));
    }

    int getCount()
    {
        return count;
    }

    void print()
    {
        PriNode<T>* current = head;
        cout << "Priority Node(front): ";

        while (current)
        {
            int x;
            cout << current->getItem(x);
            current = current->getNext();
            if (current) cout << ", ";
        }
        cout << endl;
    }

    //insert the new node in its correct position according to its priority
    void enqueue(const T& data, int priority) {
        PriNode<T>* newNode = new PriNode<T>(data, priority);

        count++;

        if (head == nullptr || priority > head->getPri()) {

            newNode->setNext(head);
            head = newNode;
            return;
        }

        PriNode<T>* current = head;
        while (current->getNext() && priority <= current->getNext()->getPri()) {
            current = current->getNext();
        }
        newNode->setNext(current->getNext());
        current->setNext(newNode);

    }

    bool dequeue(T& topEntry, int& pri) {
        if (isEmpty())
            return false;

        topEntry = head->getItem(pri);
        PriNode<T>* temp = head;
        head = head->getNext();
        delete temp;

        count--;
        return true;
    }

    bool peek(T& topEntry, int& pri) {
        if (isEmpty())
            return false;

        //topEntry = head->getItem();
        //pri = head->getPri(); 
        topEntry = head->getItem(pri);
        return true;
    }

    bool isEmpty() const {
        return head == nullptr;
    }
};
