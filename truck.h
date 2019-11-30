#pragma once
#include "printer.h"
#include "bottling_plant.h"
#include "name_server.h"

_Task Truck {
    const unsigned NUM_FLAVOURS = 4;

    Printer &printer;
	NameServer &nameServer;
	BottlingPlant &plant;
	const unsigned numVendingMachines, maxStockPerFlavour, NUM_FLAVORS=4;
	
	void main();
  public:
	Truck(Printer &prt, NameServer &nameServer, BottlingPlant &plant,
          unsigned int numVendingMachines, unsigned int maxStockPerFlavour);
};
