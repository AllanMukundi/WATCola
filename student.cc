#include "student.h"
#include "MPRNG.h"

extern MPRNG mprng;

Student::Student(Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, 
    Groupoff &groupoff, unsigned int id, unsigned int maxPurchases):
    printer(prt), nameServer(nameServer), cardOffice(cardOffice), 
    groupoff(groupoff), id(id), maxPurchases(maxPurchases) {}

void Student::freeDrink(char c, VendingMachine::Flavours favFlavour, unsigned balance) {
    yield(4);
    printer.print(Printer::Student, id, c, favFlavour, balance);
}

void Student::insufficientFunds(VendingMachine *vendingMachine, WATCard::FWATCard watCard) {
    delete watCard();
    watCard = cardOffice.transfer(id, 5 + vendingMachine->cost(), watCard());
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
    delete watCard();
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
    VendingMachine::Flavours favFlavour = (VendingMachine::Flavours)mprng(NUM_FLAVOURS-1);
    WATCard::FWATCard watCard = cardOffice.create(id, 5);
    WATCard::FWATCard giftCard = groupoff.giftCard();

    // S f ,b 
    // starting favourite soda f 
    // number of bottles b to purchase
    printer.print(Printer::Student, id, 'S', favFlavour, bottlesToPurchase);

    for (unsigned i = 0; i < bottlesToPurchase; i++) {
        // "student yields a random number of times in the range [1, 10]."
        yield(mprng(1, 10));

        VendingMachine *vendingMachine = nameServer.getMachine(id);
        // V v 
        // selecting vending machine 
        // vending machine v selected
        printer.print(Printer::Student, id, 'V', vendingMachine->getId());

        // "need to wait for money to be transferred either from the
        // WATCardOffice to their WATCard or from Groupoff to their gift card."
        _Select(watCard) {
            while (true) {
                try {
                    vendingMachine->buy(favFlavour, *watCard());
                    // B f,b 
                    // bought soda 
                    // soda flavour f purchased, WATCard balance b
                    printer.print(Printer::Student, id, 'B', favFlavour, watCard()->getBalance());
                    break;
                } catch (VendingMachine::Free &) {
                    freeDrink('A', favFlavour, watCard()->getBalance());
                    break;
                } catch (VendingMachine::Funds &) {
                    insufficientFunds(vendingMachine, watCard);
                    break;
                } catch (VendingMachine::Stock &) {
                    outOfStock(vendingMachine);
                } catch (WATCardOffice::Lost &) {
                    lostWatCard(watCard);
                }
            }
        } or _Select (giftCard) {
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

    // "Watch out for the case of a student who only buys one 
    // soda using the gift card."
    delete giftCard();
    delete watCard();

    // F 
    // finished
    printer.print(Printer::Student, id, 'F');
}
