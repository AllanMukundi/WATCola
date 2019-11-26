#pragma once
#include "vending_machine.h"

#include "printer.h"
#include "vending_machine.h"

_Task NameServer {
    Printer &printer;
    unsigned numVendingMachines, numStudents;
    VendingMachine **machines;
    unsigned *assignment;
	void main();
  public:
	NameServer( Printer & prt, unsigned int numVendingMachines, unsigned int numStudents );
    ~NameServer();
	void VMregister( VendingMachine * vendingmachine );
	VendingMachine * getMachine( unsigned int id );
	VendingMachine ** getMachineList();
};
