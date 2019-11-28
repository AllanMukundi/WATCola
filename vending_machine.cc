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
                is_restocking = false;
            } or _Accept ( inventory ) {
                printer.print(Printer::Vending, 'r');
                is_restocking = true;
            } or _When (!is_restocking) _Accept( buy ) {}
        } catch ( uMutexFailure::RendezvousFailure &e) {}
    }
    printer.print(Printer::Vending, 'F', sodaCost);
}

void VendingMachine::buy( Flavours flavour, WATCard &card) {
    if (sodaCost > card.getBalance()) {
        _Throw Funds{};
    } else if (stock[flavour] == 0) {
        _Throw Stock{};
    } else if (mprng(4) == 0) { // 1 in 5 chance this is true
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
