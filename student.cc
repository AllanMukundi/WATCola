#include "student.h"
#include "MPRNG.h"
#include <iostream>

extern MPRNG mprng;

Student::Student(Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, 
    Groupoff &groupoff, unsigned int id, unsigned int maxPurchases):
    printer(prt), nameServer(nameServer), cardOffice(cardOffice), 
    groupoff(groupoff), id(id), maxPurchases(maxPurchases) {}

void Student::freeDrink(char c, VendingMachine::Flavours favFlavour, unsigned balance) {
    yield(4);
    printer.print(Printer::Student, id, c, favFlavour, balance);
}

void Student::insufficientFunds(VendingMachine *vendingMachine, WATCard *watCard) {
    watCard = cardOffice.transfer(id, 5 + vendingMachine->cost(), watCard);
}

void Student::outOfStock(VendingMachine *&vendingMachine) {
    vendingMachine = nameServer.getMachine(id);
    // V v 
    // selecting vending machine 
    // vending machine v selected
    printer.print(Printer::Student, id, 'V', vendingMachine->getId());
}

void Student::lostWatCard(WATCard::FWATCard &watCard) {
    // L 
    // WATCard lost
    printer.print(Printer::Student, id, 'L');
    watCard.reset();
    watCard = cardOffice.create(id, 5);
}

void Student::main() {
    unsigned bottlesToPurchase = mprng(1, maxPurchases);
    VendingMachine::Flavours favouriteFlavour = (VendingMachine::Flavours)mprng(NUM_FLAVOURS-1);
    WATCard::FWATCard watCard = cardOffice.create(id, 5);

    printer.print(Printer::Kind::Student, id, 'S', (int)favouriteFlavour, bottlesToPurchase);
    WATCard::FWATCard giftcard = groupoff.giftCard();

    // buy sodas
    size_t purchased = 0;
    while (purchased < bottlesToPurchase) {
        yield(mprng(1, 10));
        VendingMachine *machine = nameServer.getMachine(id);
        printer.print(Printer::Kind::Student, id, 'V', machine->getId());
        try {
            // block until student has a card
            _Select(watCard || giftcard) {
                if (watCard.available()) {
                    // try to use watCard if available
                    WATCard *physicalcard = watCard(); // Checking if an exception is thrown

                    for (;;) {
                        try {
                        machine->buy(favouriteFlavour, *physicalcard);
                        printer.print(Printer::Kind::Student, id, 'B', physicalcard->getBalance());
                        break;
                        } catch (VendingMachine::Funds e) {
                        watCard = cardOffice.transfer(id, machine->cost() + 5, physicalcard);
                        break;
                        } catch (VendingMachine::Stock e) {
                        machine = nameServer.getMachine(id);
                        printer.print(Printer::Kind::Student, id, 'V', machine->getId());
                        }
                    }
                } else if (giftcard.available()) {
                    // try to use gift card if available
                    for (;;) {
                        try {
                            machine->buy(favouriteFlavour, *giftcard());
                            printer.print(Printer::Kind::Student, id, 'G', giftcard()->getBalance());
                            delete giftcard();
                            giftcard.reset();
                            break;
                        } catch (VendingMachine::Stock e) {
                            machine = nameServer.getMachine(id);
                            printer.print(Printer::Kind::Student, id, 'V', machine->getId());
                        }
                    }
                }
            }
            purchased += 1;
        } catch (WATCardOffice::Lost e) {
            // create new watCard if lost
            printer.print(Printer::Kind::Student, id, 'L');
            watCard.reset();
            watCard = cardOffice.create(id, 5);
        }
    }

    // Freeing (Deallocating) the dynamically allocated spaces
    _Select (watCard) {
        try {
        WATCard *physicalcard = watCard();
        delete physicalcard;
        } catch (WATCardOffice::Lost e) {
        }
    }
    _Select (giftcard) {
        WATCard *gift = giftcard();
        delete gift;
    }

    // F 
    // finished
    printer.print(Printer::Student, id, 'F');
}
