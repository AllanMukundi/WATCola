#pragma once

#include "MPRNG.h"
#include "printer.h"
#include "wat_card.h"

_Task Groupoff {
    Printer &printer;
    unsigned numStudents, sodaCost, groupoffDelay, numCards = 0;
    WATCard::FWATCard *cards;
    void main();
  public:
	Groupoff( Printer & prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay );
    ~Groupoff();
	WATCard::FWATCard giftCard();
};
