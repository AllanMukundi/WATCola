#pragma once

#include "MPRNG.h"
#include "name_server.h"
#include "wat_card.h"

_Task NameServer;

_Task VendingMachine {
	const unsigned NUM_FLAVOURS = 4;
    bool is_restocking = false;
    unsigned *stock;

    Printer &printer;
    NameServer &nameServer;
    unsigned id, sodaCost;

	void main();
  public:
	enum Flavours { BlackCherry, CreamSoda, RootBeer, Lime }; 				// flavours of soda (YOU DEFINE)
	_Event Free {};						// free, advertisement
	_Event Funds {};					// insufficient funds
	_Event Stock {};					// out of stock for particular flavour
	VendingMachine( Printer & prt, NameServer & nameServer, unsigned int id, unsigned int sodaCost );
    ~VendingMachine();
	void buy( Flavours flavour, WATCard & card );
	unsigned int * inventory();
	void restocked();
	_Nomutex unsigned int cost() const;
	_Nomutex unsigned int getId() const;
};
