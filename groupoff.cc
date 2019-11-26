#include "groupoff.h"

extern MPRNG mprng;

void Groupoff::main() {
    for(unsigned i; i < numStudents; ++i) {
        _Accept( giftCard );
    }

    while(true) {
        _Accept( ~Groupoff ) {
            break;
        } _Else {
            yield(groupoffDelay);
            unsigned randomIdx = mprng(numCards-1);
            // create and add value to card
            WATCard *realCard = new WATCard(); // callee will need to free this memory
            realCard->deposit(sodaCost);
            cards[randomIdx].delivery(realCard);
            cards[randomIdx] = cards[numCards-1]; // overwrite "deleted" card 

            printer.print(Printer::Kind::Groupoff, 'D', sodaCost);
            numCards--; // "shorten" end of array of unassigned cards
            if (numCards == 0) { break; }
        }
        printer.print(Printer::Kind::Groupoff, 'F');
    }
}

WATCard::FWATCard Groupoff::giftCard() {
    WATCard::FWATCard newCard;
    cards[numCards++] = newCard;
    return newCard;
}

Groupoff::Groupoff( Printer & prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay ):
    printer(prt), numStudents(numStudents), sodaCost(sodaCost), groupoffDelay(groupoffDelay) {
        printer.print(Printer::Kind::Groupoff, 'S');
        cards = new WATCard::FWATCard[numStudents];
    }

Groupoff::~Groupoff() {
    delete cards;
}

