#include "Scheduler.h"
void Scheduler::addToIdle(Patient* p) { idle.enqueue(p); }
void Scheduler::addToEarly(Patient* p) { early.enqueue(p); }
void Scheduler::addToLate(Patient* p) { late.enqueue(p); }

void Scheduler::addToWaitU(Patient* p) { waitU.enqueue(p); }
void Scheduler::addToWaitE(Patient* p) { waitE.enqueue(p); }
void Scheduler::addToWaitX(Patient* p) { waitX.enqueue(p); }