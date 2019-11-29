#include "vending_machine.h"

extern MPRNG mprng;

void VendingMachine::main() {
    printer.print(Printer::Vending, 'S', sodaCost);
    nameServer.VMregister(this);
    while(true) {
        try {
            _Accept( ~VendingMachine ) {
                break;
            } or _Accept ( restocked ) {
                printer.print(Printer::Vending, 'R');
                isRestocking = false;
            } or _Accept ( inventory ) {
                printer.print(Printer::Vending, 'r');
                isRestocking = true;
            } or _When (!isRestocking) _Accept( buy ) {}
        } catch ( uMutexFailure::RendezvousFailure &e) {}
    }
    printer.print(Printer::Vending, 'F');
}

void VendingMachine::buy( Flavours flavour, WATCard &card) {
    if (sodaCost > card.getBalance()) {
        _Throw Funds{};
    } else if (stock[flavour] == 0) {
        _Throw Stock{};
    } else if (mprng(4) == 0) { // 1 in 5 chance this is true
        stock[flavour]--;
        printer.print(Printer::Vending, 'B', flavour, stock[flavour]);
        _Throw Free{};
    }
    stock[flavour]--;
    card.withdraw(sodaCost);
    printer.print(Printer::Vending, 'B', flavour, stock[flavour]);
}

unsigned * VendingMachine::inventory() {
    return stock;
}

void VendingMachine::restocked() {}

unsigned VendingMachine::getId() const {
    return id;
}

unsigned VendingMachine::cost() const {
    return sodaCost;
}

VendingMachine::VendingMachine( Printer & prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost ):
    printer(prt), nameServer(nameServer), id(id), sodaCost(sodaCost) {
        stock = new unsigned[NUM_FLAVOURS];
        for(unsigned i = 0; i < NUM_FLAVOURS; ++i) {
            stock[i] = 0;
        }
    }

VendingMachine::~VendingMachine() {
    delete[] stock;
}
