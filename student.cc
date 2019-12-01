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
    // "A student begins by:
    //     - selecting a random number of bottles to purchase [1, `maxPurchases`]
    //     - a random favourite flavour [0, 3]
    //     - creates a WATCard via the `WATCardOffice` with a $5 balance, 
    //     - creates a gift card via the `Groupoff` with a value of `SodaCost`
    //     - obtains the location of a vending machine from the name server"
    unsigned bottlesToPurchase = mprng(1, maxPurchases);
    VendingMachine::Flavours favFlavour = (VendingMachine::Flavours)mprng(0, NUM_FLAVORS);
    WATCard::FWATCard watCard = cardOffice.create(id, 5);
    WATCard::FWATCard giftCard = groupoff.giftCard();
    VendingMachine *vendingMachine = nameServer.getMachine(id);

    // S f ,b 
    // starting favourite soda f 
    // number of bottles b to purchase
    printer.print(Printer::Student, id, 'S', favFlavour, bottlesToPurchase);

    // V v 
    // selecting vending machine 
    // vending machine v selected
    printer.print(Printer::Student, id, 'V', vendingMachine->getId());
    unsigned purchased = 0;
    while (purchased < bottlesToPurchase) {
        // "student yields a random number of times in the range [1, 10]."
        yield(mprng(1,10));
        try {
            _Select(watCard || giftCard) {
                if (watCard.available()) {
                    // try to use watcard if available
                    WATCard *physicalcard = watCard(); // Checking if an exception is thrown

                    for (;;) {
                        try {
                            vendingMachine->buy(favFlavour, *physicalcard);
                            printer.print(Printer::Kind::Student, id, 'B', physicalcard->getBalance());
                            break;
                        } catch (VendingMachine::Funds e) {
                            watCard = cardOffice.transfer(id, vendingMachine->cost() + 5, physicalcard);
                            break;
                        } catch (VendingMachine::Stock e) {
                            vendingMachine = nameServer.getMachine(id);
                            printer.print(Printer::Kind::Student, id, 'V', vendingMachine->getId());
                        }
                    }
                } else if (giftCard.available()) {
                    while (true) {
                        try {
                            vendingMachine->buy(favFlavour, *giftCard());
                            // G f,b 
                            // gift-card soda 
                            // soda flavour f purchased, giftcard balance b
                            printer.print(Printer::Student, id, 'G', favFlavour, giftCard()->getBalance());
                            // "Note, a giftcard future can only be used once, 
                            // so it must be reset after use to prevent any further usage"
                            giftCard.reset();
                            break;
                        } catch (VendingMachine::Free &) {
                            freeDrink('a', favFlavour, watCard()->getBalance());
                            break;
                        } catch (VendingMachine::Funds &) {
                            insufficientFunds(vendingMachine, watCard);
                            break;
                        } catch (VendingMachine::Stock &) {
                            outOfStock(vendingMachine);
                        }
                        // NOTE: no catch for `WATCardOffice::Lost` since
                        // gift cards can't be lost.
                    }
                }
            }
            purchased++;
        } catch (WATCardOffice::Lost &) {
            lostWatCard(watCard);
        }
    }

    // "Watch out for the case of a student who only buys one 
    // soda using the gift card."
    delete giftCard();
    delete watCard();

    // F 
    // finished
    printer.print(Printer::Student, id, 'F');
}
