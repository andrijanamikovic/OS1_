/*
 * main.cpp
 *
 *  Created on: Sep 4, 2021
 *      Author: OS1
 */

#include "pcb.h"
#include "dos.h"
#include <iostream.h>

#include "../h/kernel.h"
#include "syPrintf.h"



int userMain(int, char**);

int main(int argc, char** argv){
	Karnel::inic();
	int ret = userMain(argc, argv);
	Karnel::restore();
	return ret;
}

