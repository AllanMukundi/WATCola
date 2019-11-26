#pragma once

#include "bank.h"
#include "MPRNG.h"
#include "printer.h"

_Task Parent {
	Bank &bank;
	Printer &printer;
	unsigned numStudents, parentalDelay;
	void main();
  public:
	Parent(Printer &prt, Bank &bank, unsigned int numStudents, unsigned int parentalDelay);
};
