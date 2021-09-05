/*
 * main.cpp
 *
 *  Created on: Sep 4, 2021
 *      Author: OS1
 */

#include "pcb.h"
#include "karnel.h"
#include "dos.h"
#include <iostream.h>



int userMain(int, char**);

int main(int argc, char** argv){
	Karnel::inic();
	int ret = userMain(argc, argv);
	Karnel::restore();
	return ret;
}

