/*
 * PCBList.c
 *
 *  Created on: Aug 31, 2021
 *      Author: OS1
 */

#include "AList.h"

#include "karnel.h"
#include "thread.h"

List::List(){
	lock
	first = 0;
	last = 0;
	unlock
}

List::~List(){
	lock
	Elem *pom;
	while (first!=0){
		pom = first;
		first = pom->next;
		if (pom!=0)
			delete pom;
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
	return data;
}
void* List::get(){
	if (first == 0) return 0;
	else{
		Elem* temp = first;
		void* data = first->pcb;
		if (temp!=0)
			delete temp;
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
		return;
	}
	Elem* prev = temp;
	temp = temp->next;
	while (temp!=0){
		if (temp->pcb == pcb){
			prev->next = temp->next;
			if (temp!=0)
				delete temp;
			return;
		}
		prev = temp;
		temp = temp->next;
	}
}




