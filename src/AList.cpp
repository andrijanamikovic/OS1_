/*
 * PCBList.c
 *
 *  Created on: Aug 31, 2021
 *      Author: OS1
 */

#include "AList.h"

#include "../h/kernel.h"
#include "thread.h"
#include "SCHEDULE.H"
#include "pcb.h"

List::List(){
	lock
	first = 0;
	last = 0;
	size = 0;
	unlock
}

List::~List(){
	lock
	Elem *temp;
	while (first!=0){
		temp = first;
		first = temp->next;
		if (temp!=0)
			delete temp;
	}
	unlock
}


void* List::pop(){
	if (first == 0) return 0;
	Elem* temp = first;
	void* data = first->pcb;
	first = first->next;
	if (temp!=0)
		delete temp;
	if (first == 0) last = 0;
	size--;
	return data;
}
void* List::get(){
	if (first == 0) return 0;
	else{
		Elem* temp = first;
		void* data = first->pcb;
		//first = first->next;
		return data;
	}
}


void List::put(void* pcb){
	lock
	if (pcb==0) return;
	Elem* pom = new Elem(pcb);
	if (first == 0){
		first = pom;
		//last = pom;
	} else {
		if (last!=0)
			last->next = pom;
		//last = pom;

	}
	last = pom;
	size++;
	unlock
}

void List::remove(void* pcb){
	if (pcb==0) return;
	if (first == 0) return;
	Elem* temp = first;
	if (temp->pcb == pcb){
		first = first->next;
		if (temp!=0)
			delete temp;
		size--;
		return;
	}
	Elem* prev = temp;
	temp = temp->next;
	while (temp!=0){
		if (temp->pcb == pcb){
			prev->next = temp->next;
			size--;
			if (temp->next == 0) last = prev;
			return;
		}
		prev = temp;
		temp = temp->next;
	}
}

void List::unblock(){
	List::Elem* temp = first;
		List::Elem* del = first;
		while (temp!=0){
			((PCB*)(temp->pcb))->blocked = 0;
			if (temp->pcb!=0){
				lock
				Scheduler::put((PCB*)temp->pcb);
				unlock
			}
			del = temp;
			temp = temp->next;
			if (del!=0)
				delete del;
		}
		first = 0;
		last = 0;
}

int List::countSize(){
	int s = 0;
	Elem* temp = first;
	while (temp){
		s++;
		temp = temp->next;
	}
	return s;
}




