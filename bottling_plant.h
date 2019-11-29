#pragma once

#include "name_server.h"
#include "printer.h"

_Task BottlingPlant {
    Printer &printer;
    NameServer &nameServer;
    unsigned numVendingMachines, maxShippedPerFlavour, maxStockPerFlavour, timeBetweenShipments;
    const unsigned NUM_FLAVOURS = 4;
    bool isClosing = false;
    unsigned *shipment;

	void main();
  public:
	_Event Shutdown {};					// shutdown plant
	BottlingPlant( Printer & prt, NameServer & nameServer, unsigned int numVendingMachines,
				 unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
				 unsigned int timeBetweenShipments );
	void getShipment( unsigned int cargo[] );
};
