/*
 * PCBList.c
 *
 *  Created on: Aug 31, 2021
 *      Author: OS1
 */

#include "../h/AList.h"

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
	while (first){
		pom = first;
		first = pom->next;
		delete pom;
	}
	unlock
}

void* List::pop(){
	if (first ==0) return 0;
	Elem* temp = first;
	void* data = first->pcb;
	first = first->next;
	delete temp;
	if (first ==0) last = 0;
	return data;
}
void* List::get(){
	if (first ==0) return 0;
	else{
		Elem* temp = first;
		void* data = first->pcb;
		delete temp;
		return data;
	}
}


void List::put(void* pcb){
	Elem* pom = new Elem(pcb);
	if (first == 0){
		first = pom;
	} else {
		last->next = pom;
	}
	last = pom;
}

void List::remove(void* pcb){
	Elem* temp = first;
	if (temp->pcb == pcb){
		first = first->next;
		delete temp;
		return;
	}
	Elem* prev = temp;
	temp = temp->next;
	while (temp){
		if (temp->pcb == pcb){
			prev->next = temp->next;
			delete temp;
			return;
		}
		prev = temp;
		temp = temp->next;
	}
}




